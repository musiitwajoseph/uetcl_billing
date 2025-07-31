<?php 
ob_start();
set_time_limit(0);
error_reporting(null);
include "Db.php";
include "BeforeAndAfter.inc";
$customer_id = $_GET['cus'];
$peroid = $_GET['peroid'];
$db = new Db();
$current = $peroid;
$t = new BeforeAndAfter();
$tt = $t->rgf("customer", $customer_id,"customer_id", "customer_short_name");
header("Content-Type: text/csv");
header("Content-Disposition: attachment; filename=".$tt.' '.date('Y m', $current).".csv");
header("Pragma: no-cache"); 
header("Expires: 0");
$file_ending = "csv";
$file = fopen('php://output','w');  

fputcsv($file, array('Year', date('Y',$current), '', '', '', 'Action', 'ADD'));        
fputcsv($file, array('Month', date('m',$current)));        
fputcsv($file, array('Customer', $tt));        
fputcsv($file, array('Units', 'KWH'));       
fputcsv($file, array('', '')); 
fputcsv($file, array('Metering Point', 'Rate 1', 'Rate 2', 'Rate 3', 'Rate 4', 'Rate 5', 'Rate 6'));

$sql = "SELECT TOP 1 rea_date FROM r_reading WHERE rea_cus_id = '$customer_id' AND rea_date = '$current'";
$select = $db->select($sql);
extract($select[0][0]);
$sql = "SELECT * FROM r_reading, metering_point WHERE rea_cus_id = '$customer_id' AND rea_date = '$rea_date' AND rea_mp_id = mp_id ORDER BY mp_location ASC";
$select = $db->select($sql);
foreach($select[0] as $row){
    extract($row);
    $sql = "SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'";
    $sel = $db->select($sql);
    extract($sel[0][0]);
    fputcsv($file, array(strtoupper($mp_location), $rate_import_wh_1/1000, $rate_import_wh_2/1000, $rate_import_wh_3/1000, $rate_export_wh_4/1000, $rate_export_wh_5/1000, $rate_export_wh_6/1000)); 
}
