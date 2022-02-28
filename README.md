# ESPHome - Home Assistant - Ikea Ansluta Control
Creates a dimmable light components in ESPHome to interact as the Ikea Ansluta Control.
[<img src="https://esphome.io/_static/logo-text.svg" width="200"/>](https://esphome.io)
</br>
[<img src="https://www.home-assistant.io/images/home-assistant-logo.svg" width="200"/>](https://www.home-assistant.io/)

# Ansluta controller
![alt text](https://github.com/matlen67/ansluta-control/raw/master/pictures/ansluta_original.jpg "Ansluta controller")  

This project is based on:
[@matlen67 -> ansluta-control](https://github.com/matlen67/ansluta-control)

Also based on [@NDBCK -> Ansluta-Remote-Controller](https://github.com/NDBCK/Ansluta-Remote-Controller) researchs

## Hardware

- nodeMCU
- CC2500

All hardware info is based on [@matlen67 -> ansluta-control](https://github.com/matlen67/ansluta-control) project.

Check the project to detailed instructions and to get the controller address.

## Requeriments
It's needed the Ikea Ansluta Control Address bytes. After getting the address it's need to be set in `ansluta-control-ligth.h` file

```C
// pute here your AddressBytes
byte AddressByteA = 0xff;
byte AddressByteB = 0xff;
```

## installation

Copy `esphome/ansluta` folder into your `config/esphome` folder

- ansluta/cc2500_REG.h
- ansluta/cc2500_VAL.h
- ansluta/cc2500_CMD.h
- ansluta/ansluta-control.h
- ansluta/ansluta-control-light.h
  

Go to ESPHome page in home assistant installation and create a new device.

add includes and libraries
```yaml
esphome:
  name: ansluta-control-light
  comment: Ansluta Controller as dimmable light

  includes:
    - ansluta/cc2500_REG.h
    - ansluta/cc2500_VAL.h
    - ansluta/cc2500_CMD.h
    - ansluta/ansluta-control.h
    - ansluta/ansluta-control-light.h
  libraries:
    - SPI
```

and the light component:
```yaml
light:
- platform: custom
  lambda: |-
    auto light_out = new AnslutaControlLight();
    App.register_component(light_out);
    return {light_out};

  lights:
    - name: "Ansluta Light"
      default_transition_length: 0s
```


## Full yaml Example:
```yaml
esphome:
  name: ansluta-control-light
  comment: Ansluta Controller as dimmable light

  includes:
    - ansluta/cc2500_REG.h
    - ansluta/cc2500_VAL.h
    - ansluta/cc2500_CMD.h
    - ansluta/ansluta-control.h
    - ansluta/ansluta-control-light.h
  libraries:
    - SPI

esp8266:
  board: nodemcuv2

# Enable logging    
logger:

# Enable Home Assistant API
api:

ota:
  password: "aad6a56a1b6bd7192a6d53bca80f2f53"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Ansluta-Light Fallback Hotspot"
    password: "c0yRTrQM0aGm"

captive_portal:

# Example configuration entry
light:
- platform: custom
  lambda: |-
    auto light_out = new AnslutaControlLight();
    App.register_component(light_out);
    return {light_out};

  lights:
    - name: "Ansluta Light"
      default_transition_length: 0s
```


# Home Assistant
It will be auto-dicovered in Home Assistant

## Dimmable Light component

It creates a dimmable light component, but it's limmited to Ansluta Controller features. So it can only be set 0%, 50% or 100% of brightness.

So if it's set to 0% it will turn Off. Else, if the brightness is les than 75% it will set to 50%. Else 100%.