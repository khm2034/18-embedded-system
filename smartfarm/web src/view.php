<?php
$connect = mysqli_connect("localhost", "root", "sqlrootpw")
or die("Connect Fail: " . mysqli_error());
mysqli_select_db($connect, "iotfarm") or die("Select DB Fail!");

$query = "select avg(temperature) as te, avg(lightness) as li,
	substr(time, 1, 18) as ti from farm group by ti";
$result = mysqli_query($connect, $query) or die("Query Fail: " . mysqli_error());

$response = array();
$te = array();
$li = array();
$ti = array();
while ($line = mysqli_fetch_array($result, MYSQLI_ASSOC)) {
	array_push($te, $line["te"]);
	array_push($li, $line["li"]);
	array_push($ti, $line["ti"]);
}
array_push($response, $te);
array_push($response, $li);
array_push($response, $ti);
mysqli_free_result($result);
mysqli_close($connect);
echo json_encode($response);
?>