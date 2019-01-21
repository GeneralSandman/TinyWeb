<?php
class Client {
    public $name = "";
    public $email  = "";
    public $function = "";
    public $msg = "";
}
$e = new Client();

$data = file_get_contents("php://input");
if (isset($_SERVER['CONTENT_TYPE']) &&
    !strcmp($_SERVER['CONTENT_TYPE'], "text/xml")
) {
    $e = simplexml_load_string($data);
}
else {
    $e->msg = "can't support this data formate";
}

$e->function = "dynamic_post4.php";
echo json_encode($e);
?>
