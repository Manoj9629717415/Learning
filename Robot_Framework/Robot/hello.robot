*** Settings ***
Library BuiltIn

*** Variables ***
${NAME}    Manoj
@{Colours}
...  RED
...  GREEN
...  BLUE

&{USER}
...  username=Admin
...  password=12345

*** Test Cases ***
My first Test
    Log    ${NAME}

Demo
    log    Learning Robot Framework
    Should Be Equal    10    10
    Should Contain    Automobile    Auto
    Sleep   2s

Collections
    log  ${Colours}[0]
    log  ${USER.username}
    FOR  ${color}  IN  @{Colours}
        IF    $color=="RED"
            log  List Colour is ${color}
        ELSE IF    $color=="BLUE"
            log  List Colour is ${color}
        ELSE IF    $color=="GREEN"
            log  List Colour is ${color}
        ELSE
            log  UNKNOWN
        END
    END

    FOR  ${KEY}  ${VALUE}  IN  &{USER}
        log  ${KEY}=${VALUE}
    END

    ${count}=  Set Variable  1

    WHILE  ${count}<=10
         IF  ${count}/2==0
            log  even number ${count}
            CONTINUE
         ELSE
            log  odd number ${count}
         END


         IF  ${count}>8
            Log  UNKNOWN NUMER ${count}
            BREAK
         END

         ${count}=  Evaluate  ${count}+1
    END


*** Keywords ***
Welcome User
    log   Welcome to Robot Framework Training!