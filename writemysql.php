<?php
isset($_GET['wasser']) ? $wasser=$_GET['wasser'] : $wasser='';
isset($_GET['liter']) ? $liter=$_GET['liter'] : $liter='';
$pdo = new PDO('mysql:host=mysql1-rd.kt-net.at;dbname=DBSID', 'DBUSER', 'DBUSERPASSWORD');

// Daten F�llen

$datum = date("Y.m.d H:i:s");

$statement = $pdo->prepare("INSERT INTO aws_zisterne (liter, datum) VALUES (?, ?)");

$statement->execute(array($liter, $datum));


?>
