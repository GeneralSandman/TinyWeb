<?php
class Client {
    public $name = "";
    public $email  = "";
}
$e = new Client();
$data = file_get_contents("php://input");
if (isset($_SERVER['CONTENT_TYPE']) &&
    !strcmp($_SERVER['CONTENT_TYPE'], "application/json")
) {
    $e = json_decode($data);
    $e->function = "dynamic_post2.php";
    echo json_encode($e);
}
else {
    echo "can't support this data formate";
}
?>
