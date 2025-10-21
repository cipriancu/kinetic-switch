import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import remote_receiver, remote_base
from esphome.const import CONF_ID
from esphome.components import remote_receiver
from esphome.components.remote_receiver import CONF_REMOTE_RECEIVER # Importul corect

CODEOWNERS = ["@cipriancu"]
DEPENDENCIES = ["remote_receiver"]

# Definim namespace-ul C++ si clasa Sensorului
my_rf_decoder_ns = cg.esphome_ns.namespace('my_rf_decoder')
KineticSwitchSensor = my_rf_decoder_ns.class_('KineticSwitchSensor', cg.Component)

# Schema de configurare YAML
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(KineticSwitchSensor),
        cv.GenerateID(remote_base.CONF_RECEIVER_ID): cv.use_id(
            remote_receiver.RemoteReceiverComponent
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await remote_base.register_listener(var, config)
