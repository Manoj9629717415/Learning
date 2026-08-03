*** Settings ***
Library    ../Libraries/CAN.py


*** Keywords ***
Start CAN Measurement
    ${status}=    Start Measurement
    Should Be Equal
    ...    ${status}
    ...    Measurement Started

Stop CAN Measurement

    Stop Measurement

Verify CAN Measurement Running

    ${status}=    Is Measurement Running

    Should Be True
    ...    ${status}

Set Vehicle Speed

    [Arguments]
    ...    ${speed}

    Set Signal
    ...    VehicleSpeed
    ...    ${speed}

Verify Vehicle Speed

    [Arguments]
    ...    ${expected}

    ${speed}=    Get Signal
    ...    VehicleSpeed

    Should Be Equal As Integers
    ...    ${speed}
    ...    ${expected}

Accelerate Vehicle
    [Arguments]    ${accelerate}
    ${speed}=    Accelerate    ${accelerate}
    RETURN   ${speed}

Turn ON Vehicle
    ${kl_15_status}=    Set Variable
    Set Signal
    ...    KL_15
    ...    1

Turn OFF Vehicle
    Set Signal
    ...    KL_15
    ...    0
