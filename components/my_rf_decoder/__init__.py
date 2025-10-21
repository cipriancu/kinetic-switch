import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import remote_receiver, sensor
from esphome.const import CONF_ID, CONF_REMOTE_RECEIVER

CODEOWNERS = ["cipriancu"]

# Definim namespace-ul C++ si clasa Sensorului
my_rf_decoder_ns = cg.esphome_ns.namespace('my_rf_decoder')
KineticSwitchSensor = my_rf_decoder_ns.class_('KineticSwitchSensor', sensor.Sensor, cg.Component)

# Schema de configurare YAML
CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(KineticSwitchSensor),
    cv.Required(CONF_REMOTE_RECEIVER): cv.use_id(remote_receiver.RemoteReceiverComponent),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    # Generam codul pentru componenta noastra (senzorul)
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    # Obtinem referinta la remote_receiver
    remote_receiver_var = yield cg.get_variable(config[CONF_REMOTE_RECEIVER])
    
    # Setam remote_receiver-ul in clasa noastra C++
    cg.add(var.set_remote_receiver(remote_receiver_var))
