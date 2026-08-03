*** Settings ***
Resource    CANKeywords.robot
Resource    DiagnosticsKeyword.robot
Resource    ECUKeyWords.robot

*** Keywords ***
Initialize Test Environment
    Log    ===== Initializing Test Environment =====
    Start CAN Measurement

    Prepare ECU for Test


Cleanup Test Environment
    Log    ===== Cleaning Test Environment =====

    Stop CAN Measurement

    IF    '${TEST_STATUS}'== 'FAIL'
        Log    Collecting Failure Information
        READ DTC
        Log    DTC Info Saved

    END

Prepare Vehicle
    Start CAN Measurement
