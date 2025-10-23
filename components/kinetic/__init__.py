import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import remote_receiver
from esphome.const import CONF_ID

AUTO_LOAD = ["remote_base"]

kinetic_ns = cg.esphome_ns.namespace("kinetic")
RemoteReceiverListener = remote_receiver.remote_base_ns.class_("RemoteReceiverListener")

KineticComponent = kinetic_ns.class_("KineticComponent", cg.Component, RemoteReceiverListener)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(KineticComponent),
}).extend(remote_receiver.REMOTE_RECEIVER_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await remote_receiver.register_remote_receiver_listener(var, config)
