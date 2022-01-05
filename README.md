# Elrond Runtime Application (outdated/working progress)

Runtime for Elrond Modules for Linux based platforms. The runtime application is the engine of projects based on Elrond Modules. The application runtime is responsible for load and initialize the modules instances and control to everything runs fine.

## Build and run

### Requisites
First of all, you will need of build-essentials package for you distro linux. The requirements are:
 - **GNU G++**;
 - **GNU Make**;
 - **GNU Ar**;
 - **Git**;

### After cloning repository
After the repository clone, will need initialize the git submodule:
```
$ git submodule init
$ git submodule update
```

### Example test configuration
To build and run will be need a configuration file like:
```json
{
    "modules":{
        "test": "elrond::Example"
    },
    "init":{
        "test":{
            "allowLoop": true,
            "asyncLoop": true,
            "timeLoop": 1000
        }
    }
}
```
> **To more details, see [JSON configuration structure](#json-configuration-structure)**

### Running the example
With the JSON configuration saved as `config.json`, to run just type:
```
$ make run cfg=config.json
```

The output are:
```
./build/elrond-runtime config.json
 * Starting Elrond Runtime for Linux v0.0-alpha (API 0.0) by Edwino Stein <edwino.stein@gmail.com>...
 * Loading modules (1)...
  #1: Loading "test" from "elrond::Example"...
      Loaded: Example v1.0 (API 0.0) by Edwino Stein <edwino.stein@gmail.com>
      Created instance "test" of module "Example"
 * Initializing modules instances (1)...
  #1: Initializing instance "test"...
Example::onInit
 * Starting all modules instances...
Example::onStart
 * Application running (CTRL+C to stop)...
Example::loop (async)
Example::loop (async)
Example::loop (async)
 * Received Signal INT (2): Interrupt
 * Stopping all modules instances...
Example::onStop
```

> To run the runtime with dynamic linkage, you must run `export LD_LIBRARY_PATH="/full/path/to/libelrond/directory"` before the build, or install the `build/runtime-base/libelrond-runtime.so` into a library directory of your distribuition.

## JSON configuration structure

The JSON framework used to configure an Elrond application on Linux must have a root object and within it must have at least three objects with the follow keys:

 - `modules`: Object where the modules instances are defined. All instances must be declared here on pattern `"name": "module_path"`. The modules can be **internal** with prefix `elrond::`, or **external** with a path to a shared object file. To more informations about Elrond Modules see [Elrond Modules](#elrond-modules);
 - `init`: Object that associate an module instance with a **configuration object**. The pattern must be `"name":{"key1": 1234, "key2": "value"}`. Every module have your own set of configuration params, see documentation of every one;
 - `options`: Object that define extra options of the application, like the **channel managers**. In this object, can be the following params:
    * `chmgrs`: An array of objects that defines the channel managers. The parameters are:
        + `transport` (string): The name of transport module instance that will be used by this channel manager;
        + `tx` (int): Number of transmitter channels;
        + `rx` (int): Number of receiver channels;
        + `tx-fps` (int)[default = 0]: Max limit of amount of frames that will be transmitted per second. When the value is 0, no limit are defined;`*`

>  `*` In some cases, the other peer maybe can not handle certain amount of data. Eg. Serial/UART buffer of Arduino boards are limited by 64 Bytes.

### More complex example
For better understanding, a more complex example of JSON structure:

```json
{
    "modules":{
        "transport": "elrond::Loopback",
        "input": "elrond::InputToChannel",
        "led": "elrond::DigitalLed",
        "gpio": "elrond::runtime::VirtualGpio",
        "input-service": "/path/to/input/service/module.so"
    },
    "init":{
        "input": {
            "channel": 0,
            "input": 6
        },
        "led": {
            "pin": 13,
            "channel": 0
        }
    },
    "options":{
        "chmgrs":[
            {
                "transport": "transport",
                "tx": 1,
                "rx": 1
            }
        ]
    }
}
```

In this example, the `input` instance reads the `6` key from `input-service` and triggers the transmitter channel `0`. With the `transport` instance, the channel manager receives the same transmitted data. Finally, the instance `led` listens to the receiver channel `0`, and when this channel is triggered, the value is written to the GPIO pin `13` of the instance `gpio`. The output of this example is something like:

```
 * Starting Elrond Runtime for Linux v0.0-alpha (API 0.0) by Edwino Stein <edwino.stein@gmail.com>...
 * Loading modules (5)...
 (...)
 * Application running (CTRL+C to stop)...
#13 = HIGH
#13 = LOW
#13 = HIGH
#13 = LOW
 (...)
```
## Elrond modules
The Internal modules from **Elrond Common Library** are available. More details see [Elrond Common Library - Internal Modules](https://github.com/edwino-stein/elrond-common#internal-modules).

### elrond::runtime::Serial
This is a Serial/UART transport module. The module configuration is:
 - **path** (string): path to linux **tty** device;
 - **speed** (int): serial baud rate;

> This module is compatible with Arduino Serial. Due to the 64 byte buffer limitation, the channel manager recommended frame rate should be close to 10.

### elrond::runtime::Udp
Linux UDP transport module. The module configuration is:
 - **port** (int): a valid **UDP port** (0-65535). For a port less than 1024, the application needs root permission;
 - **host** (string)[default = ""]: A valid **IPv4** of the remote host. If the string is empty, the module will work in listening mode.

### elrond::runtime::VirtualGpio
Simulate a GPIO header for test environment. The module not require configuration.
