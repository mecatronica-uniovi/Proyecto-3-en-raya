Con GitHub podemos trabajar en local con archivos de un repositorio compartido, además de mantener un historial de versiones y actualizaciones.

**SUBIDA DE ARCHIVOS A GITHUB DESDE VSCODE:**

Instalar Git desde el enlace: https://git-scm.com/downloads/win. Dejar valores por defecto en las distintas ventanas de selección.
Crear una cuenta asociada a un email personal en GitHub.
Instalar Visual Studio Code y en la pestaña de Extensiones añadir Git y la librería de C/C++. Con instalar la primera es suficiente, las demás se añade al ser dependientes de forma automática.

![image](https://github.com/user-attachments/assets/98e22887-db03-45e4-aaca-e21b6f41f9c2)


Una vez hecho esto, abrimos una ventana de comandos de Windows. Aquí hay que escribir lo siguiente:

  - git config --global user.name [nombre-usuario] , siendo nombre-usuario el que os pusisteis cuando os registrasteis en GitHub.

  - git config --global user.email [email] , siendo el email el que usasteis para registraros.

Volviendo a VSCode, podemos clonar ya el repositorio. Para eso vamos al símbolo de Source Control a la izquierda de la pantalla (el 3º por arriba) y le damos a clonar repositorio. Meteis el siguiente enlace: https://github.com/mecatronica-uniovi/Proyecto-3-en-raya . Os pedirá también que creéis una carpeta en el ordenador donde almacenar también los archivos con los que trabajéis.

Os dirá que os tienen que autorizar. Cerrad sesión en GitHub si todavía la teníais abierta con vuestro correo y entrad ahora con el de mecatronicauniovi4@gmail.com y la contraseña de mecatronica_4. Ahí os saldrá en la página principal una pestaña de colaboradores. Si no, en el propio gmail de la cuenta puede llegar una solicitud, y si no, también os podeis buscar por vuestro nombre de usuario y aceptar la solicitud que os llega a vuestro email personal.
Una vez hecho esto, tiene que aparecer el repositorio clonado en el Explorador de VSCode, que en este caso se llama proyecto 3 en raya:

![image](https://github.com/user-attachments/assets/074cda31-10fa-496e-8a33-e5166eafa636)


Para crear un archivo, vamos a File y le damos a New text File. Cuando le demos nombre debemos añadir la extensión, en este caso es .cpp al tratarse de un código en C++:

![image](https://github.com/user-attachments/assets/3c2977c5-4bca-41ab-b891-5771d8b06d39)

Cuando tengamos el archivo hecho y corregido, lo subiremos al repositorio de GitHub. Para eso volveremos a la pestaña del Source Control, el propio VSCode habrá detectado que un archivo clonado ha sufrido cambios y aparecerá aquí de esta manera:

![image](https://github.com/user-attachments/assets/e052df68-9a9f-4428-a2bf-69eeb0fe77b1)

Para añadir a la cola de actualizaciones, clicamos en el botón con el símbolo del +.

![image](https://github.com/user-attachments/assets/a4bbf52f-44cb-47eb-8641-fff516f3bacb)

Ahora que ya tenemos el archivo en cola, vamos a enviarlo. Pulsamos el botón de Commit que aparece arriba y se nos abrirá un documento a la derecha. Aquí describiremos brevemente los cambios que le hemos hecho al archivo. Por último pulsaremos el tick que aparece arriba a la derecha.

![image](https://github.com/user-attachments/assets/083b3807-5067-4b85-b5a4-928dcff4a832)

A continuación, donde estaba el botón de Commit nos aparecerá uno de sincronizar cambios. Si esto no sucede, abrimos la ventana de comandos de VSCode con Ctrl+ñ y escribimos:
  - git add . (EL PUNTO INCLUIDO)
Por último hacemos el push de los archivos, es decir, el colocarlos en el repositorio de GitHub. Simplemente escribimos en la ventana de comandos:
  - git push origin branch , donde branch es la rama del repositorio en la que quieres colocar el archivo.
  - CUIDADO CON SUBIR ARCHIVOS A CARPETAS EQUIVOCADAS, ATENTOS A LA RUTA QUE USASTEIS LA ÚLTIMA VEZ
Finalizado todo esto, en GitHub veríamos lo siguiente: Una notificación del usuario que realizó la actualización con el comentario incluido en el Commit explicado que hizo. En este caso está en el main pero podría estar en la carpeta indicada en el branch, para acceder abrimos el desplegable del main y seleccionamos la que nos interesa.

![image](https://github.com/user-attachments/assets/e6169260-4f50-48e3-b16e-cd24f62d4816)

**DESCARGA DE ARCHIVOS DE GITHUB A VSCODE:**

Creamos un archivo que nos interese en el repositorio de GitHub, dentro del branch en el que deseemos trabajar. Por ejemplo dentro del branch de Trayectoria, he creado un archivo llamado Prueba para el clon:

![image](https://github.com/user-attachments/assets/35e192ce-947b-4936-821d-0637da721671)

Dentro de VSCode, debo indicarle en qué branch estoy así que escribo lo siguiente en la ventana de comandos:
  - git fetch origin , con esto actualizo los datos que tengo almacenados en mi clon por si alguno de ellos se ha visto modificado o se han creado algunos nuevos.
  - git checkout nombre-del-branch , me cambio al espacio de trabajo llamado nombre-del-branch (NO PONER ESPACIOS, LOS ESPACIOS SON GUIONES -)
De esta manera, el nuevo archivo me aparecerá en la pestaña Explorar, dentro de mi repositorio clonado:

![image](https://github.com/user-attachments/assets/ddf44715-88a0-4bde-84b2-56e89198a931)



