<?php 
error_reporting(null);
include "Db.php";
include "BeforeAndAfter.inc";

$year = $_POST['year'];
$month = $_POST['month'];
$generator = $_POST['generator'];
$narration = $_POST['narration'];
$workspace = $_POST['workspace'];

$db = new Db();

$x = array();

$db->update("mix_price",
    [
        "mix_narration"=>$narration,
        "mix_formulae"=>$workspace,
    ],
    [
        "mix_generator_id"=>$generator,
        "mix_year"=>$year,
        "mix_month"=>$month,
    ]
);

if($db->error()){
    echo '<span style="color:red;"> Error Not saved</span>';
    echo $db->error();
}else{
    echo '<span style="color:green;"> Saved</span>';    
}







