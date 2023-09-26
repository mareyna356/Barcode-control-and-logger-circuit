<?php
$conexion = mysqli_connect("localhost", "root", "root", "log_mesas", 3306);

$matricula = $_POST ['matricula'];

mysqli_query($conexion, "INSERT INTO `log_mesas`.`sjm2` (`matricula`, `entrada`) VALUES ('$matricula', CURRENT_TIMESTAMP);");

mysqli_close($conexion);

echo "Sistema activado.";
?>