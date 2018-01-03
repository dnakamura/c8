#ifndef C8_INDENTING_STREAM_HPP
#define C8_INDENTING_STREAM_HPP
#include <ostream>
#include <string>

namespace c8 {
namespace util {

class IndentingStream {
 public:
  struct NewlineMarker {};
  struct IndentMarker {};
  struct UndentMarker {};
  // struct

  IndentingStream(std::ostream& stream) : stream_(stream), indent_level_(0) {}

  template <class T>
  IndentingStream& operator<<(T const& obj) {
    stream_ << obj;
    return *this;
  }

  IndentingStream& operator<<(IndentingStream& (*func)(IndentingStream&)) {
    func(*this);
    return *this;
  }

  void indent() { ++indent_level_; }
  void undent() {
    if (indent_level_ > 0) {
      --indent_level_;
    }
  }

 private:
  std::ostream& stream_;
  unsigned indent_level_;
};

template <>
inline IndentingStream& IndentingStream::operator<<(NewlineMarker const& obj) {
  stream_ << "\n" << std::string(indent_level_ * 2, ' ');
  return *this;
}
template <>
inline IndentingStream& IndentingStream::operator<<(IndentMarker const& obj) {
  indent();
  return *this;
}
template <>
inline IndentingStream& IndentingStream::operator<<(UndentMarker const& obj) {
  undent();
  return *this;
}
// const IndentingStream::NewlineMarker newline;
}
}

#endif
