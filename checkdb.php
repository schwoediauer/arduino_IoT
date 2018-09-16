<?php
$servername = "DBSERVER";
$username = "DBUSER";
$password = "DBUSERPASSWORD";
$dbname = "DBSID";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT datum, liter FROM aws_zisterne";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "datum: " . $row["datum"]. " - liter: " . $row["liter"].  "<br>";
    }
} else {
    echo "0 results";
}
$conn->close();
?>
