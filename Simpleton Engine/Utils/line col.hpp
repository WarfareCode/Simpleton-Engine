//
//  line col.hpp
//  Game Engine
//
//  Created by Indi Kernick on 25/2/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_line_col_hpp
#define engine_utils_line_col_hpp

#include <cctype>
#include <iostream>
#include <stdexcept>

namespace Utils {
  ///A line or column is out of range
  class LineColRangeError final : public std::range_error {
  public:
    LineColRangeError();
  };

  ///Keeps track of lines and columns in text.
  ///Great for writing error messages in parsers
  template <typename Line, typename Col>
  class LineCol {
  public:
    using LineType = Line;
    using ColType = Col;
    
    static constexpr ColType  SIZE_OF_TAB = 8;
    static constexpr LineType FIRST_LINE  = 1;
    static constexpr ColType  FIRST_COL   = 1;
  
    LineCol()
      : line(FIRST_LINE), col(FIRST_COL) {}
    LineCol(const LineType line, const ColType col)
      : line(line), col(col) {
      if (line < FIRST_LINE || col < FIRST_COL) {
        throw LineColRangeError();
      }
    }
    /*
    @TODO uncomment
    
    template <typename LineT, typename ColT>
    LineCol(const LineT line, const ColT col) -> LineCol<LineT, ColT>
      : line(line), col(col) {
      if (line < FIRST_LINE || col < FIRST_COL) {
        throw LineColRangeError();
      }
    }*/
    
    ///Move line and col according to the char.
    ///Call this at the end of the loop with the char you just processed
    void putChar(const char c) {
      switch (c) {
        case '\t':
          col = (col + SIZE_OF_TAB - 1) / SIZE_OF_TAB * SIZE_OF_TAB;
          break;
        case '\n':
          line++;
          col = FIRST_COL;
          break;
        case '\v':
        case '\f':
          line++;
          break;
        case '\r':
          col = FIRST_COL;
          break;
        case '\b':
          if (col != FIRST_COL) {
            col--;
          }
          break;
        
        default:
          if (std::isprint(c)) {
            col++;
          }
      }
    }
    ///This calls putChar(char) for the first n chars in the string
    void putString(const char *str, size_t size) {
      if (str == nullptr) {
        throw std::runtime_error("Null pointer string");
      }
      while (size) {
        putChar(*str);
        str++;
        size--;
      }
    }
    ///Sets line to FIRST_LINE and col to FIRST_COL
    void reset() {
      line = FIRST_LINE;
      col = FIRST_COL;
    }
    
    LineType getLine() const {
      return line;
    }
    ColType getCol() const {
      return col;
    }
    
    const char *asStr() const {
      //The largest 64 bit unsigned integer value is 18446744073709551615 which is
      //20 characters. 20 + ':' + 20 + '\0' = 42 = the answer to the ultimate question
      static char str[42] = "LineCol::asStr is not implemented yet";
      
      /*
      @TODO uncomment
      
      std::to_chars_result end = std::to_chars(str, str + 42, line);
      if (end.ec) {
        throw std::runtime_error(end.ec.message());
      }
      *end.ptr = ':';
      end = std::to_chars(end.ptr + 1, str + 42, col);
      if (end.ec) {
        throw std::runtime_error(end.ec.message());
      }*/
      
      return str;
    }
    
    friend std::ostream &operator<<(std::ostream &stream, const LineCol &lineCol) {
      return stream << lineCol.line << ':' << lineCol.col;
    }
    
  private:
    LineType line;
    ColType col;
  };
}

#endif
