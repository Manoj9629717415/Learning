*** variables ***
${NAME}  Manoj
${AGE}   36

*** Test Cases ***
Positive Test
    Log    My Name is ${Name}
    Log    My AGE is ${AGE}
    Should be Equal   ${Name}    Manoj
    Should be Equal As Integers    ${AGE}   36

Negative Test
    Log    My Name is ${Name}
    Log    My AGE is ${AGE}
    Should be Equal   ${Name}    Yadvik
    Should be Equal As Integers    ${AGE}   2