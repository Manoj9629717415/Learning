*** settings ***
Resource    ../Resources/DiagnosticsKeyword.robot
Resource    ../Resources/CommonKeywords.robot
Variables   ../Variables/config.py


*** Test Cases ***
Validate Current SW Version
    Enter Diagnostics Session
    Verify SW Version    ${ECU_VERSION}
    Clear All DTCs