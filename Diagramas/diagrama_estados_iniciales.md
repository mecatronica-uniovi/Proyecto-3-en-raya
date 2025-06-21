stateDiagram-v2
    direction TB
    
    state INICIALIZANDO #FFFF99
    state TEST #FFFF99
    state AJUSTE #FFFF99
    state ESPERANDO #FFFF99

    [*] --> INICIALIZANDO : "0"
    [*] --> TEST : "1"
    [*] --> AJUSTE : "2"

    INICIALIZANDO --> ESPERANDO: Posición inicial calculada
    TEST --> ESPERANDO: Comando "MOV"
    AJUSTE --> ESPERANDO: Ajustes PID aplicados

