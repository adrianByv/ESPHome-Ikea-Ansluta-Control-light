/*
	esphome Ansulta Light Custom component
	
	created 28.2.2022
	by adrianByv			
*/

#include "esphome.h"

// pute here your AddressBytes
byte AddressByteA = 0xff;
byte AddressByteB = 0xff;

#include "ansluta-control.h"

AnslutaControl ansluta_control;


class AnslutaControlLight : public Component, public LightOutput {
 public:
  void setup() override {
    // This will be called by App.setup()
    ESP_LOGD("custom", "SETUP!");
    // config ChipSelect
    ansluta_control.initialize();
  }
  LightTraits get_traits() override {
    // return the traits this light supports
    auto traits = LightTraits();
    // traits.set_supported_color_modes({ColorMode::ON_OFF});
    traits.set_supported_color_modes({ColorMode::BRIGHTNESS});
    return traits;
  }

  void write_state(LightState *state) override {
    // This will be called by the light to get a new state to be written.
    float brightness;
    // use any of the provided current_values methods
    state->current_values_as_brightness(&brightness);
    // Write red, green and blue to HW
    // ...

    ESP_LOGD("custom", "BRIGHTNESS: %f", brightness);

    if (brightness == 0) {
      // turn off light
      ESP_LOGD("custom", "Light OFF");
      ansluta_control.SendCommand(AddressByteA,AddressByteB, Light_OFF);
    } else if (brightness < 0.75) {
      // turn on light 50%
      ESP_LOGD("custom", "Light ON 50%");
      ansluta_control.SendCommand(AddressByteA,AddressByteB, Light_ON_50);
    } else {
      // turn on light 100%
      ESP_LOGD("custom", "Light ON 100%");
      ansluta_control.SendCommand(AddressByteA,AddressByteB, Light_ON_100);
    }
  }

};


