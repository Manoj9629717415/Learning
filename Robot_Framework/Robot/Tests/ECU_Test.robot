*** Settings ***
Library    ../Libraries/ECU.py

*** Test Cases ***
Validate ECU SW Info
    ${sw_version}=   Get Version
    IF    $sw_version=="X315"
        log    Correct SW Present ${sw_version}
    ELSE
        log    Wrong SW Present ${sw_version}
    END

    ${temp}=    Get Temperature
    log     temperature ${temp}

Read CAN Signal
    ${speed}=    Read Signals    speed
    ${rpm}=    Read Signals    rpm
    log    speed=${speed}
    log    rpm=${rpm}
    Should Be Equal As Integers
    ...    ${speed}
    ...    100

Check Battery Voltage And Clamp Status
    ${voltage}=    Read Signals    battery_voltage
    Should Be Equal As Numbers
    ...   ${voltage}
    ...    12.4
    ${clamp}=    Read Signals    clamp_status
    Should Be Equal
    ...   ${clamp}
    ...    KL_15 OFF
    Toggle Clamp
    ${clamp}=    Read Signals    clamp_status
    Should Be Equal
    ...   ${clamp}
    ...    KL_15 ON

Check Engine Temperature
    ${eng_temp}=    Read Signals    engine_temp
    Should Be True    ${eng_temp}<110
    log    Engine Temperature Normal
