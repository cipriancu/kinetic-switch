# kinetic-switch
esphome custom components for kinetic switch

Yalm config:

external_components:
  - source: github://cipriancu/kinetic-switch@main
    components: [my_rf_decoder]

remote_receiver:
  pin: GPIO18 # Pinul la care este conectat receptorul 433MHz
  # dump: raw 

# Definirea senzorului custom
sensor:
  - platform: my_rf_decoder 
    name: "Switch Kinetic ID"
    id: kinetic_id_sensor
    remote_receiver: rx_id
