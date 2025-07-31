<?php 

session_start();

function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
}

include "../classes/Db.php";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

error_reporting(0);
$t = new BeforeAndAfter();
$efris = new Efris();

$db = new Db();
$sql = "SELECT * FROM m";


echo $sql = "select rate_id as rateid, rate_cus_id as ratecusid, rea_mp_id as reampid FROM r_rate, r_reading where rea_date = '1720990800' AND rea_id = rate_reading_id ORDER BY rate_id DESC;"; 
//$sql = "select rate_cus_id,rate_sfi,rate_sfe,rate_lfe,rate_lfi,rate_narration,rate_label,rate_wc_sfe,rate_wc_sfi,rate_wc_lfe,rate_wc_lfi,rate_payable_imports,rate_payable_exports,rate_tlf,rate_tlfs from r_rate, r_reading where rea_date = '1718398800' AND rea_id = rate_reading_id ORDER BY rate_id DESC;"; 
$select = $db->select($sql);

foreach($select[0] as $row){
	extract($row);

	$d = new Db();
	$s = $d->select("select rate_cus_id,rate_sfi,rate_sfe,rate_lfe,rate_lfi,rate_narration,rate_label,rate_wc_sfe,rate_wc_sfi,rate_wc_lfe,rate_wc_lfi,rate_payable_imports,rate_payable_exports,rate_tlf,rate_tlfs from r_rate, r_reading where rea_date = '1718398800' AND rea_mp_id = '$reampid' AND rea_id = rate_reading_id ORDER BY rate_id DESC;");
	if($d->num_rows())
	extract($s[0][0]);

	echo "UPDATE r_rate SET rate_sfi = '$rate_sfi', rate_sfe = '$rate_sfe', rate_lfe = '$rate_lfe', rate_lfi = '$rate_lfi', rate_narration = '$rate_narration', rate_label = '$rate_label', rate_wc_sfe = '$rate_wc_sfe', rate_wc_sfi = '$rate_wc_sfi', rate_wc_lfe = '$rate_wc_lfe', rate_wc_lfi = '$rate_wc_lfi', rate_payable_imports = '$rate_payable_imports', rate_payable_exports = '$rate_payable_exports', rate_tlf = '$rate_tlf', rate_tlfs = '$rate_tlfs' where rate_cus_id = '$ratecusid' AND rate_id = '$rateid';<br/>";


}


