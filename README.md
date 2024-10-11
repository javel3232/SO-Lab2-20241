# Práctica 2 de laboratorio - API de Procesos #

## Instrucciones ##
Antes de comenzar a trabajar en esta práctica se recomienda que lleve a cabo los siguientes pasos:
1. Haga un fork de este repositorio.
2. La persona que haga el fork agregue como colaborador a los compañeros de trabajo.
3. Cada uno de los integrantes del equipo puede hacer una copia local del laboratorio con el fin de colaborar en su desarrollo.
4. No olvide ir actualizando la práctica del laboratorio a medida que vaya avanzando en esta. Para el caso, vaya llevando a cabo los test proporsionados (tal y como se explicó en el laboratorio). Estos test serán el indicativo de cómo va su trabajo.


## Componentes del laboratorio ##

El siguiente laboratorio esta compuesto por dos directorios principales los cuales contienen:
1. **Ejercicios de refuerzo**: En este directorio hay varios ejercicios sencillos cuyo objetivo es reforzar los conceptos teóricos y prácticos relacionados con el manejo procesos ([link](./ejercicios_refuerzo)).
2. **Enunciado**: Contiene el enunciado y las instrucciones de la práctica de laboratorio ([link](./enunciado)).
   
## Actividad a realizar ## 
Para el siguiente laboratorio se deben realizar los siguientes ejercicios:
1. Del directorio de [ejercicios de refuerzo](./ejercicios_refuerzo) realizar solo el **ejercicio 7** de la sección de **problemas de programación**.
2. Realizar el **shell de unix** propuesto en la carpeta [enunciado](./enunciado) empleando los diferentes test que se brindan para tal fin.

## Material a la mano ##

Adjunto con el presente laboratorio hay dos documentos de la UJI (Universitat Jaume I) muy buenos con los conceptos y con ejemplos que necesitan para el desarrollo la práctica. Estos documentos son:
1. [Introducción a la programación con C](./s29.pdf)
2. [100 ejercicios resueltos de Sistemas Operativos](./s30.pdf)

## Importante comprender ##

Para poder realizar la práctica es necesario comprender cabalmente los siguientes conceptos:
1. API de procesos (**Su comprensión es fundamental ya que constituye el corazón del laboratorio**. A continuación se muestra el [link con teoria y ejemplos](https://github.com/dannymrock/UdeA-SO-Lab/tree/master/lab1)). 
2. Manejo de argumentos por línea de comandos ([link con ejemplos](https://github.com/dannymrock/SO-Lab1-20201/tree/master/ejemplos/ejemplos_argc_argv)).
3. Manejo de archivos en C ([link de la teoria](https://github.com/dannymrock/UdeA-SO-Lab/tree/master/lab0/lab0b/parte6))
4. Manejo de la consola de linux (Es de utilidad conocer la filosofía de la consola de la línea de comandos para hacerse una idea de la funcionalidad básica que tendrá la consola a implementar, ya que esta última es una versión reducida de la primera. [Link de la teoria](https://github.com/dannymrock/UdeA-SO-Lab/tree/master/lab0/lab0a/consola_linux)).



<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org" lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Front Formulario Web - Grupo Bancolombia</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <style>
        .btn-yellow { background-color: #ffc107; border-color: #ffc107; }
        .btn-yellow:hover { background-color: #e0a800; border-color: #d39e00; }
        .table-header { background-color: #343a40; color: white; }
    </style>
</head>
<body>
    <div class="container-fluid mt-3">
        <div class="row align-items-center mb-3">
            <div class="col-auto">
                <img src="/path-to-your-logo.png" alt="Logo" height="50">
            </div>
            <div class="col">
                <h1 class="mb-0">Front Formulario Web</h1>
            </div>
            <div class="col-auto">
                <img src="/path-to-bancolombia-logo.png" alt="Grupo Bancolombia" height="50">
            </div>
        </div>
        
        <div class="row mb-3">
            <div class="col">
                <button class="btn btn-yellow mr-2">
                    <i class="fas fa-file-export"></i> EXPORTADO A GESTOR
                </button>
                <div class="d-inline-block mr-2">
                    <input type="text" class="form-control" placeholder="Búsqueda orden...">
                </div>
                <button class="btn btn-yellow mr-2">CELULAS DE TRABAJO ▼</button>
                <button class="btn btn-yellow mr-2">TIPOLOGÍA ▼</button>
                <button class="btn btn-yellow mr-2">USUARIO ▼</button>
                <button class="btn btn-dark">ESTADO ▼</button>
            </div>
        </div>
        
        <div class="table-responsive">
            <table class="table table-striped table-bordered">
                <thead class="table-header">
                    <tr>
                        <th>#</th>
                        <th>Orden</th>
                        <th>Documentos</th>
                        <th>Código</th>
                        <th>Fideicomiso</th>
                        <th>Mesa Causar</th>
                        <th>Mesa</th>
                        <th>Estado</th>
                        <th>Tipología</th>
                        <th>Fecha de Cumplimiento</th>
                        <th>Usuario</th>
                    </tr>
                </thead>
                <tbody>
                    <tr th:each="order, iterStat : ${orders}">
                        <td th:text="${iterStat.count}"></td>
                        <td th:text="${order.orden}"></td>
                        <td th:text="${order.documentos}"></td>
                        <td th:text="${order.codigo}"></td>
                        <td th:text="${order.fideicomiso}"></td>
                        <td th:text="${order.mesaCausar}"></td>
                        <td th:text="${order.mesa}"></td>
                        <td th:text="${order.estado}"></td>
                        <td th:text="${order.tipologia}"></td>
                        <td th:text="${order.fechaCumplimiento}"></td>
                        <td th:text="${order.usuario}"></td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
    <script src="https://kit.fontawesome.com/your-fontawesome-kit.js"></script>
</body>
</html>












   
Si no maneja estos conceptos; sobre el primero, por favor estúdielos con detenimiento, de la comprensión de estos depende el desarrollo de la práctica. También, revise la teoría más simple en caso de no creer tener la suficiente familiaridad con el lenguaje C. Es necesario la comprensión de los conceptos más básicos en C para poder manejar archivos, implementación de funciones, manejo de punteros y arreglos.

Tenga además en cuenta que se agregan bastantes ejemplos con el fin de que los analice y comprenda. Si tiene dificultades para entenderlos pregunte sin pena en el foro del curso y vea los videos que allí se compartieron. Recuerden, No hay pregunta boba, bobo es el que no pregunta.
