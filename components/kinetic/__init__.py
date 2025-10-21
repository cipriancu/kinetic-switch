import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, remote_receiver
from esphome.const import CONF_ID

kinetic_ns = cg.esphome_ns.namespace("kinetic")
KineticSwitchSensor = kinetic_ns.class_("KineticSwitchSensor", sensor.Sensor, cg.Component)

CONF_RECEIVER_ID = "receiver_id"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(KineticSwitchSensor),
    cv.Required(CONF_RECEIVER_ID): cv.use_id(remote_receiver.RemoteReceiverComponent),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    rcvr = await cg.get_variable(config[CONF_RECEIVER_ID])
    cg.add(var.set_remote_receiver(rcvr))
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
