//
//  rect.hpp
//  Game Engine
//
//  Created by Indi Kernick on 28/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_rect_hpp
#define engine_math_rect_hpp

#include "dir.hpp"
#include <iostream>
#include "scale.hpp"
#include <glm/vec2.hpp>

namespace Math {
  template <typename T>
  class RectPS;

  ///A rectangle defined by a top-left point and a bottom-right point
  template <typename T>
  class RectPP {
  
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  
  public:
    using Scalar = T;
    using Vector = glm::tvec2<Scalar>;
  
    static constexpr Scalar EPSILON = std::is_integral<Scalar>::value ? Scalar(1) : Scalar(0);
    static const RectPP NORM_0_1;
    static const RectPP LARGEST;
    
    RectPP()
      : tl(Scalar(0), Scalar(0)), br(-EPSILON, -EPSILON) {}
    RectPP(const RectPP &) = default;
    RectPP(RectPP &&) = default;
    ~RectPP() = default;
    
    RectPP &operator=(const RectPP &) = default;
    RectPP &operator=(RectPP &&) = default;
    
    explicit RectPP(const Vector size)
      : tl(Scalar(0), Scalar(0)), br(size - EPSILON) {}
    RectPP(const Vector tl, const Vector br)
      : tl(tl), br(br) {}
    RectPP(const Scalar left, const Scalar top, const Scalar right, const Scalar bottom)
      : tl(left, top), br(right, bottom) {}
    
    template <typename U>
    explicit RectPP(const RectPP<U> other)
      : tl(static_cast<Scalar>(other.tl.x),
           static_cast<Scalar>(other.tl.y)),
        br(static_cast<Scalar>(other.br.x + RectPP<U>::EPSILON) - EPSILON,
           static_cast<Scalar>(other.br.y + RectPP<U>::EPSILON) - EPSILON) {}
    
    template <typename U>
    explicit RectPP(const RectPS<U> other)
      : tl(static_cast<Scalar>(other.p.x),
           static_cast<Scalar>(other.p.y)),
        br(static_cast<Scalar>(other.p.x + other.s.x) - EPSILON,
           static_cast<Scalar>(other.p.y + other.s.y) - EPSILON) {}
    
    template <typename U>
    explicit operator RectPP<U>() const {
      return {
        {
          static_cast<U>(tl.x),
          static_cast<U>(tl.y)
        },
        {
          static_cast<U>(br.x + EPSILON) - RectPP<U>::EPSILON,
          static_cast<U>(br.y + EPSILON) - RectPP<U>::EPSILON
        }
      };
    }
    
    template <typename U>
    explicit operator RectPS<U>() const {
      return {
        {
          static_cast<U>(tl.x),
          static_cast<U>(tl.y)
        },
        {
          static_cast<U>(br.x - tl.x + EPSILON),
          static_cast<U>(br.y - tl.y + EPSILON)
        }
      };
    }
    
    bool operator==(const RectPP other) const {
      return tl == other.tl && br == other.br;
    }
    bool operator!=(const RectPP other) const {
      return tl != other.tl || br != other.br;
    }
    
    //add and substract mean translate
    RectPP &operator+=(const Vector other) {
      tl += other;
      br += other;
      return *this;
    }
    RectPP &operator-=(const Vector other) {
      tl -= other;
      br -= other;
    }
    //multiply and divide mean scale
    RectPP &operator*=(const Vector other) {
      tl *= other;
      br *= other;
      return *this;
    }
    RectPP &operator/=(const Vector other) {
      tl /= other;
      br /= other;
      return *this;
    }
    
    //add and substract mean translate
    RectPP operator+(const Vector other) const {
      return {
        tl + other,
        br + other
      };
    }
    RectPP operator-(const Vector other) const {
      return {
        tl - other,
        br - other
      };
    }
    //multiply and divide mean scale
    RectPP operator*(const Vector other) const {
      return {
        tl * other,
        br * other
      };
    }
    RectPP operator/(const Vector other) const {
      return {
        tl / other,
        br / other
      };
    }
    
    RectPP scale(const RectPP from, const RectPP to) const {
      return {
        Math::scale(left, from.left, from.right, to.left, to.right),
        Math::scale(top, from.top, from.bottom, to.top, to.bottom),
        Math::scale(right, from.left, from.right, to.left, to.right),
        Math::scale(bottom, from.top, from.bottom, to.top, to.bottom)
      };
    }
    
    template <typename U>
    RectPP<U> convert(const RectPP from, const RectPP<U> to) const {
      using Type = std::common_type_t<T, U>;
      return {
        Math::scale<Type>(left, from.left, from.right, to.left, to.right),
        Math::scale<Type>(top, from.top, from.bottom, to.top, to.bottom),
        Math::scale<Type>(right + EPSILON, from.left, from.right, to.left, to.right) - RectPP<U>::EPSILON,
        Math::scale<Type>(bottom + EPSILON, from.top, from.bottom, to.top, from.bottom) - RectPP<U>::EPSILON
      };
    }
    
    template <typename U, typename V>
    RectPP<U> convertMul(const glm::tvec2<V> scale) const {
      using Type = std::common_type_t<T, U, V>;
      return {
        static_cast<U>(static_cast<Type>(left) * scale.x),
        static_cast<U>(static_cast<Type>(top) * scale.y),
        static_cast<U>(static_cast<Type>(right + EPSILON) * scale.x) - RectPP<U>::EPSILON,
        static_cast<U>(static_cast<Type>(bottom + EPSILON) * scale.y) - RectPP<U>::EPSILON
      };
    }
    
    template <typename U, typename V>
    RectPP<U> convertDiv(const glm::tvec2<V> scale) const {
      using Type = std::common_type_t<T, U, V>;
      return {
        static_cast<U>(static_cast<Type>(left) / scale.x),
        static_cast<U>(static_cast<Type>(top) / scale.y),
        static_cast<U>(static_cast<Type>(right + EPSILON) / scale.x) - RectPP<U>::EPSILON,
        static_cast<U>(static_cast<Type>(bottom + EPSILON) / scale.y) - RectPP<U>::EPSILON
      };
    }
    
    Scalar side(const Math::Dir dir) const {
      switch (dir) {
        case Math::Dir::UP:
          return top;
        case Math::Dir::RIGHT:
          return right;
        case Math::Dir::DOWN:
          return bottom;
        case Math::Dir::LEFT:
          return left;
        default:
          assert(false);
      }
    }
    
    void side(const Math::Dir dir, const Scalar val) {
      switch (dir) {
        case Math::Dir::UP:
          top = val;
          break;
        case Math::Dir::RIGHT:
          right = val;
          break;
        case Math::Dir::DOWN:
          bottom = val;
          break;
        case Math::Dir::LEFT:
          left = val;
          break;
        default:
          assert(false);
      }
    }
    
    Vector topLeft() const {
      return tl;
    }
    Vector topRight() const {
      return {br.x, tl.y};
    }
    Vector bottomRight() const {
      return br;
    }
    Vector bottomLeft() const {
      return {tl.x, br.y};
    }
    
    void topLeft(const Vector val) {
      tl = val;
    }
    void topRight(const Vector val) {
      br.x = val.x;
      tl.y = val.y;
    }
    void bottomRight(const Vector val) {
      br = val;
    }
    void bottomLeft(const Vector val) {
      tl.x = val.x;
      br.y = val.y;
    }
    
    Vector size() const {
      return (br - tl) + EPSILON;
    }
    
    bool sizeZero() const {
      return (br.x - tl.x) + EPSILON == Scalar(0) &&
             (br.y - tl.y) + EPSILON == Scalar(0);
    }
    
    ///Move top left corner to make size equal to val
    void sizeTopLeft(const Vector val) {
      tl = (br - val) + EPSILON;
    }
    ///Move top right corner to make size equal to val
    void sizeTopRight(const Vector val) {
      br.x = (tl.x + val.x) - EPSILON;
      tl.y = (br.y - val.y) + EPSILON;
    }
    ///Move bottom right corner to make size equal to val
    void sizeBottomRight(const Vector val) {
      br = (tl + val) - EPSILON;
    }
    ///Move bottom left corner to make size equal to val
    void sizeBottomLeft(const Vector val) {
      tl.x = (br.x - val.x) + EPSILON;
      br.y = (tl.y + val.y) - EPSILON;
    }
    
    ///Move bottom right corner to make size equal to val
    void size(const Vector val) {
      sizeBottomRight(val);
    }
    
    bool interceptsWith(const RectPP other) const {
      return tl.x           < other.br.x + EPSILON &&
             tl.y           < other.br.y + EPSILON &&
             br.x + EPSILON > other.tl.x           &&
             br.y + EPSILON > other.tl.y;
    }
    bool encloses(const RectPP other) const {
      return tl.x <= other.tl.x &&
             tl.y <= other.tl.y &&
             br.x >= other.br.x &&
             br.y >= other.br.y;
    }
    bool encloses(const Vector other) const {
      return tl.x <= other.x &&
             tl.y <= other.y &&
             br.x >= other.x &&
             br.y >= other.y;
    }
    
    friend std::ostream &operator<<(std::ostream &stream, const RectPP rect) {
      return stream << "RectPP {{" << rect.left << ", " << rect.top << "}, {" << rect.right << ", " << rect.bottom << "}}";
    }
    
    #pragma clang diagnostic push 
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic ignored "-Wnested-anon-types"
  
    union {
      struct {
        //top left corner
        Vector tl;
        //bottom right corner
        Vector br;
      };
      struct {
        Scalar left;
        Scalar top;
        Scalar right;
        Scalar bottom;
      };
    };
    
    #pragma clang diagnostic pop
  };
  
  template <typename T>
  const RectPP<T> RectPP<T>::NORM_0_1 = {T(0), T(0), T(1) - EPSILON, T(1) - EPSILON};
  
  template <typename T>
  const RectPP<T> RectPP<T>::LARGEST = {
    std::numeric_limits<T>::lowest(),
    std::numeric_limits<T>::lowest(),
    std::numeric_limits<T>::max(),
    std::numeric_limits<T>::max()
  };
  
  ///A rectangle defined by a top-left point and a size
  template <typename T>
  class RectPS {
  
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  
  public:
    using Scalar = T;
    using Vector = glm::tvec2<Scalar>;
  
    static constexpr Scalar EPSILON = std::is_integral<Scalar>::value ? Scalar(1) : Scalar(0);
    static const RectPS NORM_0_1;
    static const RectPS LARGEST;
  
    RectPS() = default;
    RectPS(const RectPS &) = default;
    RectPS(RectPS &&) = default;
    ~RectPS() = default;
    
    RectPS &operator=(const RectPS &) = default;
    RectPS &operator=(RectPS &&) = default;
    
    explicit RectPS(const Vector size)
      : p(T(0), T(0)), s(size) {}
    RectPS(const Vector pos, const Vector size)
      : p(pos), s(size) {}
    RectPS(const Scalar px, const Scalar py, const Scalar sx, const Scalar sy)
      : p(px, py), s(sx, sy) {}
    
    template <typename U>
    explicit RectPS(const RectPS<U> other)
      : p(static_cast<T>(other.p.x), static_cast<T>(other.p.y)),
        s(static_cast<T>(other.s.x), static_cast<T>(other.s.y)) {}
    
    template <typename U>
    explicit RectPS(const RectPP<U> other)
      : p(static_cast<T>(other.tl.x),
          static_cast<T>(other.tl.y)),
        s(static_cast<T>(other.br.x - other.tl.x + RectPP<U>::EPSILON),
          static_cast<T>(other.br.y - other.tl.y + RectPP<U>::EPSILON)) {}
    
    template <typename U>
    explicit operator RectPS<U>() const {
      return {
        {static_cast<U>(p.x), static_cast<U>(p.y)},
        {static_cast<U>(s.x), static_cast<U>(s.y)}
      };
    }
    
    template <typename U>
    explicit operator RectPP<U>() const {
      return {
        {
          static_cast<U>(p.x),
          static_cast<U>(p.y)
        },
        {
          static_cast<U>(p.x + s.x) - RectPP<U>::EPSILON,
          static_cast<U>(p.y + s.y) - RectPP<U>::EPSILON,
        }
      };
    }
    
    bool operator==(const RectPS other) const {
      return p == other.p && s == other.s;
    }
    bool operator!=(const RectPS other) const {
      return p != other.p || s != other.s;
    }
    
    //add and substract mean translate
    RectPS &operator+=(const Vector other) {
      p += other;
      return *this;
    }
    RectPS &operator-=(const Vector other) {
      p -= other;
    }
    //multiply and divide mean scale
    RectPS &operator*=(const Vector other) {
      p *= other;
      s *= other;
      return *this;
    }
    RectPS &operator/=(const Vector other) {
      p /= other;
      s /= other;
      return *this;
    }
    
    //add and substract mean translate
    RectPS operator+(const Vector other) const {
      return {
        p + other,
        s
      };
    }
    RectPS operator-(const Vector other) const {
      return {
        p - other,
        s
      };
    }
    //multiply and divide mean scale
    RectPS operator*(const Vector other) const {
      return {
        p * other,
        s * other
      };
    }
    RectPS operator/(const Vector other) const {
      return {
        p / other,
        s / other
      };
    }
    
    RectPS scale(const RectPS from, const RectPS to) const {
      return {
        {
          Math::scale(p.x, from.p.x, from.p.x + from.s.x, to.p.x, to.p.x + to.s.x),
          Math::scale(p.y, from.p.y, from.p.y + from.s.y, to.p.y, to.p.y + to.s.y),
        },
        {
          Math::scale(s.x, T(0), from.s.x, T(0), to.s.x),
          Math::scale(s.y, T(0), from.s.y, T(0), to.s.y)
        }
      };
    }
    
    template <typename U>
    RectPS<U> convert(const RectPS from, const RectPS<U> to) const {
      using Type = std::common_type_t<T, U>;
      return {
        {
          static_cast<U>(Math::scale<Type>(
            p.x, from.p.x, from.p.x + from.s.x, to.p.x, to.p.x + to.s.x
          )),
          static_cast<U>(Math::scale<Type>(
            p.y, from.p.y, from.p.y + from.s.y, to.p.y, to.p.y + to.s.y
          ))
        },
        {
          static_cast<U>(Math::scale<Type>(
            s.x, T(0), from.s.x, T(0), to.s.x
          )),
          static_cast<U>(Math::scale<Type>(
            s.y, T(0), from.s.y, T(0), to.s.y
          )),
        }
      };
    }
    
    template <typename U, typename V>
    RectPS<U> convertMul(const glm::tvec2<V> scale) const {
      using Type = std::common_type_t<T, U, V>;
      return {
        {
          static_cast<U>(static_cast<Type>(p.x) * scale.x),
          static_cast<U>(static_cast<Type>(p.y) * scale.y)
        },
        {
          static_cast<U>(static_cast<Type>(s.x) * scale.x),
          static_cast<U>(static_cast<Type>(s.y) * scale.y)
        }
      };
    }
    
    template <typename U, typename V>
    RectPS<U> convertDiv(const glm::tvec2<V> scale) const {
      using Type = std::common_type_t<T, U, V>;
      return {
        {
          static_cast<U>(static_cast<Type>(p.x) / scale.x),
          static_cast<U>(static_cast<Type>(p.y) / scale.y)
        },
        {
          static_cast<U>(static_cast<Type>(s.x) / scale.x),
          static_cast<U>(static_cast<Type>(s.y) / scale.y)
        }
      };
    }
    
    Scalar left() const {
      return p.x;
    }
    Scalar top() const {
      return p.y;
    }
    Scalar right() const {
      return p.x + s.x - EPSILON;
    }
    Scalar bottom() const {
      return p.y + s.y - EPSILON;
    }
    
    void left(const Scalar val) {
      p.x = val;
    }
    void top(const Scalar val) {
      p.y = val;
    }
    void right(const Scalar val) {
      s.x = val - p.x + EPSILON;
    }
    void bottom(const Scalar val) {
      s.y = val - p.y + EPSILON;
    }
    
    Scalar side(const Math::Dir dir) const {
      switch (dir) {
        case Math::Dir::UP:
          return top();
        case Math::Dir::RIGHT:
          return right();
        case Math::Dir::DOWN:
          return bottom();
        case Math::Dir::LEFT:
          return left();
        default:
          assert(false);
      }
    }
    
    void side(const Math::Dir dir, const Scalar val) {
      switch (dir) {
        case Math::Dir::UP:
          top(val);
          break;
        case Math::Dir::RIGHT:
          right(val);
          break;
        case Math::Dir::DOWN:
          bottom(val);
          break;
        case Math::Dir::LEFT:
          left(val);
          break;
        default:
          assert(false);
      }
    }
    
    bool interceptsWith(const RectPS other) const {
      return p.x       < other.p.x + other.s.x &&
             p.y       < other.p.y + other.s.y &&
             p.x + s.x > other.p.x             &&
             p.y + s.y > other.p.y;
    }
    bool encloses(const RectPS other) const {
      return p.x       <= other.p.x             &&
             p.y       <= other.p.y             &&
             p.x + s.x >= other.p.x + other.s.x &&
             p.y + s.y >= other.p.y + other.s.y;
    }
    bool encloses(const Vector other) const {
      return p.x                 <= other.x &&
             p.y                 <= other.y &&
             p.x + s.x - EPSILON >= other.x &&
             p.y + s.y - EPSILON >= other.y;
    }
    
    friend std::ostream &operator<<(std::ostream &stream, const RectPS rect) {
      return stream << "RectPP {{" << rect.p.x << ", " << rect.p.y << "}, {" << rect.s.x << ", " << rect.s.y << "}}";
    }
    
    Vector p;
    Vector s;
  };
  
  template <typename T>
  const RectPS<T> RectPS<T>::NORM_0_1 = {{T(0), T(0)}, {T(1), T(1)}};
  
  template <typename T>
  const RectPS<T> RectPS<T>::LARGEST = {
    {
      std::numeric_limits<T>::lowest(),
      std::numeric_limits<T>::lowest()
    },
    {
      std::numeric_limits<T>::max(),
      std::numeric_limits<T>::max()
    }
  };
  
  template <typename T>
  glm::tvec2<T> scale(const glm::tvec2<T> point, const RectPP<T> from, const RectPP<T> to) {
    return {
      Math::scale(point.x, from.left, from.right, to.left, to.right),
      Math::scale(point.y, from.top, from.bottom, to.top, to.bottom)
    };
  }
  
  template <typename T>
  glm::tvec2<T> scale(const glm::tvec2<T> point, const RectPS<T> from, const RectPS<T> to) {
    return {
      Math::scale(point.x, from.p.x, from.p.x + from.s.x, to.p.x, to.p.x + to.s.x),
      Math::scale(point.y, from.p.y, from.p.y + from.s.y, to.p.y, to.p.y + to.s.y)
    };
  }
  
  template <typename T>
  glm::tvec2<T> scale(const glm::tvec2<T> point, const glm::tvec2<T> from, const glm::tvec2<T> to) {
    return {
      Math::scale(point.x, 0, from.x, 0, to.x),
      Math::scale(point.y, 0, from.y, 0, to.y)
    };
  }
}
#endif
