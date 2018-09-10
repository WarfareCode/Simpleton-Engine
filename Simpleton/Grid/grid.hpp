//
//  grid.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 21/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_grid_grid_hpp
#define engine_grid_grid_hpp

#include <array>
#include <vector>
#include "pos.hpp"
#include "../Utils/numeric iterators.hpp"

namespace Grid {
  namespace detail {
    template <typename Derived>
    class GridBase {
    private:
      Derived &that() {
      	return *static_cast<Derived *>(this);
      }
      const Derived &that() const {
      	return *static_cast<const Derived *>(this);
      }

      using Tile = typename Derived::Tile;

      void checkRange(const Pos pos) const {
        if (that().outOfRange(pos)) {
          throw std::range_error("Position out of range");
        }
      }
      void checkRange(const size_t index) const {
        if (that().outOfRange(index)) {
          throw std::range_error("Index out of range");
        }
      }
      
    public:
      auto begin() {
        return that().mTiles.begin();
      }
      auto begin() const {
        return that().mTiles.begin();
      }
      auto cbegin() const {
        return that().mTiles.begin();
      }
      auto end() {
        return that().mTiles.end();
      }
      auto end() const {
        return that().mTiles.end();
      }
      auto cend() const {
        return that().mTiles.end();
      }
      
      auto hori() const {
        return Utils::range(that().size().x);
      }
      auto vert() const {
        return Utils::range(that().size().y);
      }
      auto horiRev() const {
        return Utils::rangeRev(that().size().x);
      }
      auto vertRev() const {
        return Utils::rangeRev(that().size().y);
      }

      Tile &operator()(const Coord x, const Coord y) {
        return that().mTiles[that().toIndex({x, y})];
      }
      const Tile &operator()(const Coord x, const Coord y) const {
        return that().mTiles[that().toIndex({x, y})];
      }
      Tile &operator[](const Pos pos) {
        return that().mTiles[that().toIndex(pos)];
      }
      const Tile &operator[](const Pos pos) const {
        return that().mTiles[that().toIndex(pos)];
      }
      Tile &at(const Pos pos) {
        checkRange(pos);
        return that().mTiles[that().toIndex(pos)];
      }
      const Tile &at(const Pos pos) const {
        checkRange(pos);
        return that().mTiles[that().toIndex(pos)];
      }
      
      Tile &operator[](const size_t index) {
        return that().mTiles[index];
      }
      const Tile &operator[](const size_t index) const {
        return that().mTiles[index];
      }
      Tile &at(const size_t index) {
        checkRange(index);
        return that().mTiles[index];
      }
      const Tile &at(const size_t index) const {
        checkRange(index);
        return that().mTiles[index];
      } 
    };
  }

  template <typename Tile_, Coord Width_ = 0, Coord Height_ = 0>
  class Grid final : public detail::GridBase<Grid<Tile_, Width_, Height_>> {
  public:
    friend detail::GridBase<Grid<Tile_, Width_, Height_>>;
  
    using Tile = Tile_;
    static constexpr Coord Width = Width_;
    static constexpr Coord Height = Height_;
    using Tiles = std::array<Tile, Width * Height>;
    
    static_assert(Width > 0);
    static_assert(Height > 0);

    Grid(Grid &&) = default;
    Grid &operator=(Grid &&) = default;
    
    Grid() = default;
    // explicit Grid(const Tile &tile)

    void fill(const Tile &tile) {
      mTiles.fill(tile);
    }

    static constexpr Pos size() {
      return {Width, Height};
    }
    static constexpr Coord width() {
      return Width;
    }
    static constexpr Coord height() {
      return Height;
    }
    static constexpr Coord area() {
      return Width * Height;
    }
    
    bool outOfRange(const Pos pos) const {
      return pos.x >= Width || pos.y >= Height;
    }
    bool outOfRange(const size_t index) const {
      return index >= mTiles.size();
    }

    static size_t toIndex(const Pos pos) {
      return pos.y * Width + pos.x;
    }
    static Pos toPos(const size_t index) {
      const Coord cindex = static_cast<Coord>(index);
      return {cindex % Width, cindex / Width};
    }
  
  private:
    Tiles mTiles;
  };

  template <typename Tile_>
  class Grid<Tile_, 0, 0> final : public detail::GridBase<Grid<Tile_, 0, 0>> {
  public:
    friend detail::GridBase<Grid<Tile_, 0, 0>>;
  
    using Tile = Tile_;
    using Tiles = std::vector<Tile>;
  
    Grid(Grid &&) = default;
    Grid &operator=(Grid &&) = default;
  
    Grid()
      : mSize(0, 0) {}
    explicit Grid(const Pos size, const Tile &tile = {})
      : mTiles(size.x * size.y, tile), mSize(size) {}
    
    void clear() {
      mTiles.clear();
      mSize = {0, 0};
    }
    void resize(const Pos size, const Tile &tile = {}) {
      mTiles.resize(size.x * size.y, tile);
      mSize = size;
    }
    void fill(const Tile &tile) {
      std::fill(mTiles.begin(), mTiles.end(), tile);
    }
    
    Pos size() const {
      return mSize;
    }
    Coord width() const {
      return mSize.x;
    }
    Coord height() const {
      return mSize.y;
    }
    Coord area() const {
      return static_cast<Coord>(mTiles.size());
    }
    
    bool outOfRange(const Pos pos) const {
      return pos.x >= mSize.x || pos.y >= mSize.y;
    }
    bool outOfRange(const size_t index) const {
      return index >= mTiles.size();
    }
    
    size_t toIndex(const Pos pos) const {
      return pos.y * mSize.x + pos.x;
    }
    Pos toPos(const size_t index) const {
      const Coord cindex = static_cast<Coord>(index);
      return {cindex % mSize.x, cindex / mSize.x};
    }
  
  private:
    Tiles mTiles;
    Pos mSize;
  };
}

#endif
