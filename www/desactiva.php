<?php
$conexion = mysqli_connect("localhost", "root", "root", "log_mesas", 3306);

mysqli_query($conexion, "UPDATE `log_mesas`.`sjm2` SET `salida` = CURRENT_TIMESTAMP WHERE `id` = (SELECT * FROM (SELECT MAX(`id`) from `log_mesas`.`sjm2`) AS A)");

mysqli_close($conexion);

echo "Sistema desactivado.";
?>