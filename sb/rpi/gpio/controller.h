#ifndef sb_rpi_gpio_controller_h
#define sb_rpi_gpio_controller_h

#include <stdint.h>

namespace sb {
namespace rpi {
namespace gpio {

/// pin function
enum class mode
{
   input  =0,
   output =1,
   alt0   =4,
   alt1   =5,
   alt2   =6,
   alt3   =7,
   alt4   =3,
   alt5   =2
};
using mode_t=mode;

/// pull up/down mode
enum class pud_mode
{
   none =0,
   down =1,
   up   =2
};
using pud_mode_t=pud_mode;



class controller
{
public:
   /// constructor
   controller();
   /// destructor
   virtual ~controller();

   void set_mode(unsigned gpio_pin, const mode_t& mode);
   void set_mode(unsigned gpio_pin, const pud_mode_t& mode);
   void set_mode(unsigned gpio_pin, const mode_t& function_mode, const pud_mode_t& pud_mode);

   void write(unsigned gpio_pin, bool val);
   bool read(unsigned gpio_pin);

protected:
   void initialize();

private:
   enum class state
   {
      initial,
      open_error,
      mapping_error,
      read_only,
      read_write,
   };
   using state_t = state;

   uint32_t* m_register;
   state_t m_state;
};

}  // namespace gpio
}  // namespace rpi
}  // namespace sb

#include <sb/rpi/gpio/detail/controller.ipp>

#endif
