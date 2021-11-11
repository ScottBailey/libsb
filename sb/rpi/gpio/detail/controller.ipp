#ifndef sb_rpi_gpio_controller_h
# error "This file must be included by and only by sb/rpi/gpio/controller.h"
#endif

#include <sys/mman.h>  // mmap(), munmap()
#include <fcntl.h>     // open()
#include <unistd.h>    // close()

#include <exception>   // std::exception

namespace sb {
namespace rpi {
namespace gpio {


// file statics ------------------------------------------------------------------------------------------------------

// @note It might be more appropriate to move these to static private class members...

// length of the register mapping
static const size_t MAP_LENGTH(0xB4);


static const size_t CLEAR0(10);
static const size_t SET0(7);

static const size_t CLEAR1(11);       //?
static const size_t SET1(8);          //?

static const size_t READ0(13);
static const size_t READ1(14);        //?

static const size_t PUD_OFFSET(37);
static const size_t PUD_CLOCK0_OFFSET(38);
static const size_t PUD_CLOCK1_OFFSET(39);  //?


inline size_t bank(const unsigned& gpio_pin)
{
   return gpio_pin >> 5;
}


inline uint32_t bit(const unsigned& gpio_pin)
{
   return (1<<(gpio_pin&0x1F));
}

//--- constructr/destructor ------------------------------------------------------------------------------------------

inline controller::controller()
   : m_register(nullptr)
   , m_state(state::initial)
{
   initialize();
}


inline controller::~controller()
{
   if(m_register)
      munmap(m_register,MAP_LENGTH);
}

//--- alphabetic -----------------------------------------------------------------------------------------------------

inline void controller::initialize()
{
#if defined(IGNORE_GPIO)
   return;
#endif

   bool read_only=false;

   int fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
   if(fd < 0)
   {
      read_only = false;
      fd = open("/dev/gpiomem", O_RDONLY | O_SYNC);
      if(fd < 0)
      {
         m_state = state::open_error;
         return;
      }
   }

   void* mmap_return;
   if( read_only )
      mmap_return = mmap(nullptr, MAP_LENGTH, PROT_READ, MAP_SHARED, fd, 0);
   else
      mmap_return = mmap(nullptr, MAP_LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
   close(fd);

   if( mmap_return == MAP_FAILED )
   {
      m_state = state::mapping_error;
      return;
   }

   m_register = reinterpret_cast<uint32_t*>(mmap_return);
   if( read_only )
      m_state = state::read_only;
   else
      m_state = state::read_write;
}


inline bool controller::read(unsigned gpio_pin)
{
#if defined(IGNORE_GPIO)
   return true;
#endif

   uint32_t* read_addr = m_register + READ0 + bank(gpio_pin);
   return (*read_addr & bit(gpio_pin) );
}


inline void controller::set_mode(unsigned gpio_pin, const mode_t& pin_mode)
{
#if defined(IGNORE_GPIO)
   return;
#endif

   if( m_state != state::read_write )
      throw std::exception();

   size_t reg = gpio_pin/10;
   size_t shift = gpio_pin%10 *3;

   m_register[reg] = (m_register[reg] & ~(7<<shift)) | (uint32_t(pin_mode)<<shift);
}


inline void controller::set_mode(unsigned gpio_pin, const pud_mode_t& mode)
{
#if defined(IGNORE_GPIO)
   return;
#endif

   if( m_state != state::read_write )
      throw std::exception();

   uint32_t* pud_addr = m_register + PUD_OFFSET;
   uint32_t* gpio_addr = m_register + PUD_CLOCK0_OFFSET + bank(gpio_pin);

   *pud_addr = uint32_t(mode);

   usleep(20);
   *gpio_addr = bit(gpio_pin);

   usleep(20);
   *pud_addr = 0;
   *gpio_addr = 0;
}


inline void controller::set_mode(unsigned gpio_pin, const mode_t& direction_mode, const pud_mode_t& pud_mode)
{
   set_mode(gpio_pin,direction_mode);
   set_mode(gpio_pin,pud_mode);
}


inline void controller::write(unsigned gpio_pin, bool val)
{
#if defined(IGNORE_GPIO)
   return;
#endif

   if( m_state != state::read_write )
      throw std::exception();

   uint32_t* addr = m_register + bank(gpio_pin);
   if( val )
      addr += SET0;
   else
      addr += CLEAR0;
   *addr = bit(gpio_pin);
}

}  // namespace gpio
}  // namespace rpi
}  // namespace sb
