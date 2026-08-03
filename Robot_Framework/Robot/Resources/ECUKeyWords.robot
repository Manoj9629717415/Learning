*** Settings ***
Library    ../Libraries/ECU.py
Variables    ../Variables/config.py

*** Keywords ***
Verify SW Version
    ${sw_version}=    Get Version
    Should Be Equal
    ...   ${ECU_VERSION}
    ...   ${sw_version}

Read Vehicle Speed
    ${speed}=    Read Signals    speed
    RETURN   ${speed}

Switch Clamp
    Toggle Clamp

Check Battery OK
    ${battery}=    Read Signals    battery_voltage
    Should Be True    ${battery}<=${BATTERY_LIMIT}

Check Engine Temperature OK
    ${temp}=    Read Signals    engine_temp
    Should Be True    ${temp}<=${MAX_ENGINE_TEMP}

Prepare ECU for Test
    Verify SW Version
    Switch Clamp
    Check Battery OK

Read VIN Number
    ${vin}=    Read Vin
    RETURN    ${vin}