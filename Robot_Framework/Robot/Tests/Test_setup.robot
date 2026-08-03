*** Settings ***
Resource    ../Resources/TestExecutionSuite.robot

Suite Setup    Initialize Test Environment
Suite Teardown    Cleanup Test Environment

Test Setup    Enter Extended Session

*** Test Cases ***
Verify Vehicle Speed
    Set Vehicle Speed    100
    Verify Vehicle Speed    100
