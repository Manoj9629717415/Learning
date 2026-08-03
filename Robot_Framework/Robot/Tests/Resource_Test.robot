*** Settings ***
Resource    ../Resources/ECUKeyWords.robot
Resource    ../Resources/CommonKeywords.robot

*** Test Cases ***
Validate ECU
    Print Test Start    Validate ECU
    Verify SW Version

    ${speed}=    Read Vehicle Speed
    Log    speed=${speed}

    Check Battery OK

    Check Engine Temperature OK

    Switch Clamp
    Print Test End    Validate ECU


