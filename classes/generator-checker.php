<?php 
error_reporting(null);
include "Db.php";
include "BeforeAndAfter.inc";

$year = $_POST['year'];
$month = $_POST['month'];
$generator = $_POST['generator'];

$db = new Db();

$x = array();
$sql = "SELECT * FROM mix_price WHERE mix_year = '$year' AND mix_month = '$month' AND mix_generator_id = '$generator'";
$select = $db->select($sql);
extract($select[0][0]);

$usd=$eur=$ugx=$ex_eur=$ex_usd=0;

if($db->num_rows()){
	$usd=$eur=$ugx=$ex_eur=$ex_usd=0;

	if($mix_amount != 0) $usd = 1;
	if($mix_amount2 != 0) $eur = 1;
	if($mix_amount3 != 0) $ugx = 1;
	if($mix_amount2 != 0) $ex_eur = 1;
	if($mix_amount3 != 0|| $mix_amount2 != 0) $ex_usd = 1;

}

$x = array(
		'narration'=>$mix_narration,
		'workspace'=>$mix_formulae,
		'fields'=>$t,
		'usd'=>$usd,
		'eur'=>$eur,
		'ugx'=>$ugx,
		'ex_eur'=>$ex_eur,
		'ex_usd'=>$ex_usd,
	);

echo json_encode($x);







