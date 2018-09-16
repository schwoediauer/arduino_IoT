<html>
<head>
<title>Regenwasserstand via MySQL</title>
</head>
<body bgcolor="#FFFFFF">
<?php
$datum = date("d.m.Y");
$uhrzeit = date("H:i");
echo $datum," - ",$uhrzeit," Uhr";
?>
<h1>Wasserstand der Regenzisterne</h1>
<b>Regenwasser Max. 7.000 Liter</B>
<p align="left">

<?php
$pdo = new PDO('mysql:host=mysql1-rd.kt-net.at;dbname=DBSID', 'DBUSER', 'DBUSERPASSWORD');

$statement = $pdo->prepare("SELECT COUNT(*) AS anzahl FROM aws_zisterne");
$statement->execute();
$row = $statement->fetch();
echo "Es wurden <b>".$row['anzahl']." </b>Einträge gefunden";

echo "<table border=\"2\" cellpadding=\"3\" cellspacing=\"1\">";
echo "<tr bgcolor=\"#D7D7D7\"><td><b>Datum</b></td>";
echo "<td><b>Liter</b></td>";
echo "<td><b>Balken</b></td>";
echo "<td><b>%</b></td></tr>";

$sql = "SELECT * FROM aws_zisterne ORDER BY datum DESC LIMIT 20";
foreach ($pdo->query($sql) as $row) {


echo "<tr><td width=\"170\">".$row['datum'].
"</td>";
echo "<td width=\"60\">".$row['liter'].
"</td>";

$literprozent = $row['liter'] * 100 / 7000;
$literbalken = $literprozent * 3;
$literrounded = number_format($literprozent,2);
echo "<td width=\"300\"><img src=\"balken.png\" width=\"$literbalken\" height=\"11\" title=\"$literprozent %\" alt=\"$literprozent %\"/></td>";
echo "<td>$literrounded %</td></tr>";}

?>

</p>
</body>
</html>
