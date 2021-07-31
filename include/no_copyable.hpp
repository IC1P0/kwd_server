#ifndef __OB_NO_COPYABLE_H__
#define __OB_NO_COPYABLE_H__

namespace OB
{
class NoCopyable {
public:
  NoCopyable () {}
  NoCopyable(const NoCopyable &tmp) = delete;
  NoCopyable &operator=(const NoCopyable &tmp) = delete;
};
}//end of namespace Th

#endif
