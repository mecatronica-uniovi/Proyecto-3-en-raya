@echo off
cd d CUsersPatriOneDrive - Universidad de OviedoEscritorioMASTERProyecto-3-en-rayaTest_robot

echo ---------------------------------
echo Eliminando configuración Git anterior...
rd s q .git

echo ---------------------------------
echo Inicializando nuevo repositorio Git...
git init

echo ---------------------------------
echo Agregando archivos...
git add .

echo ---------------------------------
echo Realizando commit...
git commit -m Subida inicial de archivos desde Test_robot al branch Test-control

echo ---------------------------------
echo Añadiendo repositorio remoto...
git remote add origin httpsgithub.commecatronica-unioviProyecto-3-en-raya.git

echo ---------------------------------
echo Creando branch Test-control...
git checkout -b Test-control

echo ---------------------------------
echo Subiendo archivos al branch Test-control (forzado)...
git push -u origin Test-control --force

echo ---------------------------------
echo ¡Listo! Los archivos se han subido al branch Test-control.
pause
