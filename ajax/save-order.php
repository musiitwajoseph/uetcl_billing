<?php 
include "../classes/init.inc";

$where = $_POST['where'];
$number = $_POST['number'];
$range = $_POST['range'];

 $db = new Db();
 $delete = $db->query("DELETE FROM master_approval_orders WHERE app_type = '$where'");

 $setter = 1;

 $items = '';
 for ($i = 0; $i < $number; $i++) {

     $items .= '> ' . $item = $_POST['role'.$i];
     $items .= '> ' . $action = $_POST['action' . $i];
     $items .= '> ' . $realtimeUpload = $_POST['realtimeUpload' . $i];
     $items .= '> ' . $upload = $_POST['upload' . $i];
     $items .= '> ' . $action = $_POST['action' . $i];

     $items .= '> ' . $Approve_type = $_POST['Approve_type' . $i];
     $items .= '> ' . $Reject_type = $_POST['Reject_type' . $i];
     $items .= '> ' . $SendBack_type = $_POST['SendBack_type' . $i];
     $items .= '> ' . $OnHold_type = $_POST['OnHold_type' . $i];

     $items .= '> ' . $Approve_type_label = $_POST['Approve_type_label' . $i];
     $items .= '> ' . $Reject_type_label = $_POST['Reject_type_label' . $i];
     $items .= '> ' . $SendBack_type_label = $_POST['SendBack_type_label' . $i];
     $items .= '> ' . $OnHold_type_label = $_POST['OnHold_type_label' . $i];

     $items .= '> ' . $waitingTime = $_POST['waitingTime' . $i];
     $items .= '> ' . $waitingTimeUnit = $_POST['waitingTimeUnit'. $i];
     // $items .= '> ' . $action = $_POST['action' . $i];

     if ($item) {

         $db = new Db();

         $db->insert("master_approval_orders", [
             "app_count" => $setter, "app_type" => $where,
             "app_role_id" => $item,
             "app_range" => $range,
             "app_action" => $action,
             "app_signature" => $realtimeUpload,
             "app_attachment" => $upload,
             
             "app_btn_reject" => $Reject_type,
             "app_btn_approve" => $Approve_type,
             "app_btn_amend" => $SendBack_type,
             "app_btn_onhold" => $OnHold_type,

             "app_btn_reject_label" => $Reject_type_label,
             "app_btn_approve_label" => $Approve_type_label,
             "app_btn_amend_label" => $SendBack_type_label,
             "app_btn_onhold_label" => $OnHold_type_label,

             "app_waiting_time" => $waitingTime,
             "app_waiting_time_unit" => $waitingTimeUnit,

             "app_date_added" => time(),
             "app_added_by" => 11, //Helper::user_id(),
             ]
         );
     }

     $setter++;
 }

