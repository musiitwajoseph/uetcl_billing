<?php 
error_reporting(null);
include "Db.php";
include "BeforeAndAfter.inc";

$year = $_POST['year'];
$month = $_POST['month'];
$date = strtotime($year.'-'.$month.'-15');
$cus_id = $_POST['customer_id'];
$metering_point = $_POST['mp_id'];
$note = $_POST['note'];

$db = new Db();

if(empty($date)){
        //sdafaf
    if($note == "credit-note")
        $sel = $db->select("SELECT TOP 1 n_rea_id,n_ rea_date FROM r_reading_cn WHERE n_rea_cus_id = $cus_id AND n_rea_mp_id = '$metering_point' ORDER BY n_rea_date DESC");
    else
        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");

        extract($sel[0][0]);
    }else{
        $year = date('Y', @$date);
        $month = date('m', @$date);


        if($note == "credit-note")
            $sel = $db->select("SELECT n_rea_id as x, n_rea_date as y FROM r_reading_cn WHERE n_rea_cus_id = $cus_id  AND n_rea_mp_id = '$metering_point'");
        else
            $sel = $db->select("SELECT rea_id as x, rea_date as y FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point'");
        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
        foreach($sel[0] as $row){
            extract($row);
            if($year == date('Y', $y) && $month == date('m', $y) ){
                $rea_id = $x;
                $rea_date = $y;
                //echo 'reached';
            }
        }
    }


    //rate_import_wh_1 as rat1, rate_import_wh_2 as rat2, rate_import_wh_3 as rat3,rate_export_wh_4 as rat4,rate_export_wh_5 as rat5,rate_export_wh_6 as rat6, rate_cus_id, rate_advance_1 as ais, rate_advance_2 as aip, rate_advance_3 as aio, rate_advance_4 as aes, rate_advance_5 as aep, rate_advance_6 as aeo, rate_swap
    $db = new Db();

    if($note == "credit-note")
        $sel = $db->select("SELECT TOP 1 n_rate_narration as rate_narration, n_rate_lfi as rate_lfi, n_rate_lfe as rate_lfe, n_rate_label as rate_label, n_rate_wheeling_charge as rate_wheeling_charge, n_rate_wc_lfi as rate_wc_lfi, n_rate_wc_lfe as rate_wc_lfe, n_rate_tlf as rate_tlf FROM r_rate_cn WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id'");
    else
        $sel = $db->select("SELECT TOP 1 rate_narration, rate_lfi, rate_lfe, rate_label, rate_wheeling_charge, rate_wc_lfi, rate_wc_lfe, rate_tlf FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");

    extract($sel[0][0]);


$x = array(
		'narration'=>$rate_narration,
		'workspace'=>$rate_lfi,
		'workspace2'=>$rate_lfe,
        'newLabel'=>$rate_label,
        'wheelingCharge'=>$rate_wc_lfi,
        'wheelingChargeE'=>$rate_wc_lfe,
        'tlf'=>$rate_tlf,
	);

echo json_encode($x);







