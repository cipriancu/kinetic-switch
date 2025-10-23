# kinetic-switch
esphome custom components for kinetic switch

Yalm config:

external_components:
  - source: github://cipriancu/kinetic-switch
    components: [ kinetic ]

remote_receiver:
- id: rx_id
  pin: $rx # GDO2
  dump: raw



# Definirea senzorului custom
kinetic:
  id: kinetic_component
  receiver_id: rx_id
