*** Settings ***
Library    ../Libraries/Diagnostics.py
Variables    ../Variables/config.py

*** Keywords ***
Verify Session
    [Arguments]    ${expected_session}
    ${current_session}=    Get Session
    Should be Equal As Numbers
    ...    ${current_session}
    ...    ${expected_session}


Enter Session Control
    Change Session    ${SESSION_CONTROL}
    Verify Session    ${SESSION_CONTROL}

Enter Tester Present
    Change Session    ${TESTER_PRESENT}
    Verify Session    ${TESTER_PRESENT}

Enter Extended Session
    Change Session    ${EXTENDED_SESSION}
    Verify Session    ${EXTENDED_SESSION}

Enter Diagnostics Session
    Enter Extended Session
    Enter Tester Present

Clear All DTCs
    ${status}=    Clear DTC
    Should be Equal
    ...    ${status}
    ...    DTC Cleared

READ DTC
    ${dtc}=    Get Dtc
    RETURN    ${dtc}

Create Diagnostic Fault
    [Arguments]    ${expected_dtc}
    ADD DTC    ${expected_dtc}

Verify SW Version
    [Arguments]    ${expected}
    ${current}=    Get Sw Version
    Should be Equal
    ...    ${current}
    ...    ${expected}