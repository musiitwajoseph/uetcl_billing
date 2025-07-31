<?php 
error_reporting(null);
date_default_timezone_set("Africa/Kampala");
include "Db.php";
include "Field_calculate.inc";
include "BeforeAndAfter.inc";
include "AuditTrail.inc";
include "AccessRights.inc";

$t = new BeforeAndAfter();

$year = $_POST['year'];
$month = $_POST['month'];
$date = strtotime($year.'-'.$month.'-15');
$cus_id = $_POST['customer_id'];
$metering_point = $_POST['mp_id'];

$narration = $_POST['narration'];
$lfi = $_POST['workspace'];
$lfe = $_POST['workspace2'];
$sfe = $_POST['sfe'];
$sfi = $_POST['sfi'];
$wc_sfe = $_POST['wc_sfe'];
$wc_sfi = $_POST['wc_sfi'];
$newLabel = $_POST['newLabel'];
$wheelingCharge = $_POST['wheelingCharge'];
$wheelingChargeE = $_POST['wheelingChargeE'];
$tlf = $_POST['tlf'];
$tlfs = $_POST['tlfactor'];
$note = $_POST['note'];

$imports = str_replace("([","", end(explode('=>', $wc_sfi)));
$imports = str_replace("])","", $imports);

$exports = str_replace("([","", end(explode('=>', $wc_sfe)));
$exports = str_replace("])","", $exports);

$db = new Db();
$sql = "SELECT customer_id as imports FROM customer WHERE customer_short_name = '$imports'";
$select = $db->select($sql);
extract($select[0][0]);

$db = new Db();
$sql = "SELECT customer_id as exports FROM customer WHERE customer_short_name = '$exports'";
$select = $db->select($sql);
extract($select[0][0]);

//if($)

$db = new Db();

if(empty($date)){
        //sdafaf
        if($note == "credit-note"){
            $sel = $db->select("SELECT TOP 1 n_rea_id, n_rea_date FROM r_reading_cn WHERE n_rea_cus_id = $cus_id AND n_rea_mp_id = '$metering_point' ORDER BY n_rea_date DESC");
            extract($sel[0][0]);
        }else{
            $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
            extract($sel[0][0]);
        }
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
if($rea_id){
    $db = new Db();
    if($note == "credit-note"){
        $sql = "UPDATE r_rate_cn SET n_rate_narration = '$narration',n_rate_wc_lfe = '$wheelingChargeE',n_rate_wc_lfi = '$wheelingCharge',n_rate_wc_sfe = '$wc_sfe', n_rate_wc_sfi = '$wc_sfi', n_rate_lfi='$lfi',  n_rate_lfe='$lfe',  n_rate_sfi='$sfi', n_rate_sfe='$sfe',  n_rate_label='$newLabel',  n_rate_wheeling_charge='$wheelingCharge', n_rate_payable_imports = '$imports', n_rate_payable_exports = '$exports', n_rate_tlf = '$tlf', n_rate_tlfs='$tlfs' WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id'";
            
            $db->query($sql);
    }else{
        $sql = "UPDATE r_rate SET rate_narration = '$narration',rate_wc_lfe = '$wheelingChargeE',rate_wc_lfi = '$wheelingCharge',rate_wc_sfe = '$wc_sfe', rate_wc_sfi = '$wc_sfi', rate_lfi='$lfi',  rate_lfe='$lfe',  rate_sfi='$sfi', rate_sfe='$sfe',  rate_label='$newLabel',  rate_wheeling_charge='$wheelingCharge', rate_payable_imports = '$imports', rate_payable_exports = '$exports', rate_tlf = '$tlf', rate_tlfs='$tlfs' WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'";

        $db->query($sql);

        $db = new Db();
        $rrr = $rea_id;

        //=====================================================================
        //checking which part to use for advance
        //=====================================================================
        $aaa = array();
        
        $customer_id = $cus_id;
        $tou = $t->tou($customer_id, $month, $year, 1);

        $peak = $tou[0];
        $shoulder = $tou[1];
        $off_peak = $tou[2];

        //imports ========================================================
        $rate_sfe = $t->rgf("r_rate", $rrr, "rate_reading_id", "rate_sfe");
        $v = $t->get_advance($customer_id, $mp_id, $month, $year, $rate_sfe);

        $g0 = ($v['AR1']);
        $g1 = ($v['AR2']);
        $g2 = ($v['AR3']);
        $g3 = ($v['ATOTAL']);

        $aaa["tisu"] = $g0;
        $aaa["tipu"] = $g1;
        $aaa["tiou"] = $g2;
        $aaa["tiu"] = $g3;

        $aaa["tisa"] = $g0*$shoulder;
        $aaa["tipa"] = $g1*$peak;
        $aaa["tioa"] = $g2*$off_peak;
        $aaa["tia"] = $g0*$shoulder + $g1*$peak + $g2*$off_peak ;
        //export =====================================================          
        $rate_sfi = $t->rgf("r_rate", $rrr, "rate_reading_id", "rate_sfi");
        $ve = $t->get_advance($customer_id, $mp_id, $month, $year, $rate_sfi);

        $g0e = ($ve['AR1'])?($ve['AR1']):0;
        $g1e = ($ve['AR2'])?($ve['AR2']):0;
        $g2e = ($ve['AR3'])?($ve['AR3']):0;
        $g3e = ($ve['ATOTAL'])?($ve['ATOTAL']):0;

        $aaa["tesu"] = $g0e;
        $aaa["tepu"] = $g1e;
        $aaa["teou"] = $g2e;
        $aaa["teu"] = $g3e;

        $aaa["tesa"] = $g0e*$shoulder;
        $aaa["tepa"] = $g1e*$peak;
        $aaa["teoa"] = $g2e*$off_peak;
        $aaa["tea"] = $g0e*$shoulder + $g1e*$peak + $g2e*$off_peak ;

        // //imports WHEELING CHARGE =====================================================
        $rate_wc_sfi = $t->rgf("r_rate", $rrr, "rate_reading_id", "rate_wc_sfi");
        $wc = $t->get_advanceWheeling($customer_id, $mp_id, $month, $year, $rate_wc_sfi,$customer_id);

        $aaa["wc_tisu"] = (double)$wc['AR1'];
        $aaa["wc_tipu"] = (double)$wc['AR2'];
        $aaa["wc_tiou"] = (double)$wc['AR3'];
        $aaa["wc_tiu"] = (double)$wc['ATOTAL'];

        $aaa["wc_tisa"] = (double)$wc['AMR1'];;
        $aaa["wc_tipa"] = (double)$wc['AMR2'];
        $aaa["wc_tioa"] = (double)$wc['AMR3'];
        $aaa["wc_tia"] = (double)$wc['AMR1'] + (double)$wc['AMR2'] + (double)$wc['AMR3'];


        $db = new Db();
        if(1){ //imports as advance
            $select = $db->update("r_rate", $aaa, ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);
        }else{ //exports as advance
            // $select = $db->update("r_rate",[
            //  //$variable=>$value,
            //  "rate_added_by"=>$user_id,
            //  "rate_date_added"=>time(),
            // ], ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);
            // echo $db->error();
        }

    }

// echo $db->error();

    // $db->update("r_rate",[
    //     "rate_narration"=>$narration,
    //     "rate_lfi"=>$lfi,
    //     "rate_lfe"=>$lfe,
    //     "rate_label"=>$newLabel,
    //     "rate_sfi"=>$sfi,
    //     "rate_sfe"=>$sfe,
    // ],["rate_reading_id"=>$rea_id, "rate_cus_id"=>$cus_id]);
$x = array();
if($db->error()){
    $x=array("status"=>"Error", "message"=>$db->error());
}else{
    $x=array("status"=>"Saved", "message"=>"Saved");    
}
}else{

    $x=array("status"=>"Error", "message"=>"Not Saved. Readings do not exist."); 
}
echo json_encode($x);





