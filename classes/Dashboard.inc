<?php 

class Dashboard extends BeforeAndAfter{
	
	public function getLinks(){
		
		$links = array(
			array()
		);
		
		return $links;
	}
	
	public function index(){
        error_reporting(1);


            
    
      
        

       // echo strtotime('2023-09-15')
        //1694725200
        //READING ID'S ARE CREATED THE SYSTEM

 //        $db = new Db();

 //        $k = 27796;//last reading id
 //       $select = $db->select("SELECT * FROM r_reading WHERE rea_date='1694725200' AND rea_cus_id='2019'");
 //       foreach ($select[0] as $r) {
 //           extract($r);
 //           // $reading_id = 27796+$k;
 //           $k++;

 //       $db1 = new Db();
 //       $select1 = $db1->select("SELECT * FROM r_rate WHERE rate_cus_id='2019' AND rate_reading_id='$rea_id'");
 //           $v = "";

 //        foreach($select1[0] as $row){
 //            extract($row);
           
 //            $mp_customer_id = 2036;
 //            $time = time();
 //            $v .= "INSERT INTO [dbo].[r_rate]
 //           ([rate_cus_id]
 //           ,[rate_import_wh_1]
 //           ,[rate_import_wh_2]
 //           ,[rate_import_wh_3]
 //           ,[rate_date_added]
 //           ,[rate_export_wh_5]
 //           ,[rate_added_by]
 //           ,[rate_reading_id])
 //     VALUES
 //           ('$mp_customer_id'
 //           ,'$rate_export_wh_4'
 //           ,'$rate_export_wh_5'
 //           ,'$rate_export_wh_6'
 //           ,'$time'
 //           ,'0.0'
 //           ,'10'
 //         ,'$k'); ";
 //        }

 //       // echo $v;

       
	// }


    // $db = new Db();
    //     $sql = "SELECT * FROM metering_point WHERE mp_customer_id = '2019'";
    //     $select = $db->select($sql);

    //     $v = "";
    //     foreach($select[0] as $row){
    //         extract($row);
    //         $mp_customer_id = 2036;
    //         $time = time();
    //         $v .= "INSERT INTO [dbo].[metering_point]
    //        ([mp_location]
    //        ,[mp_main_meter]
    //        ,[mp_check_meter]
    //        ,[mp_customer_id]
    //        ,[mp_date_added]
    //        ,[mp_added_by]
    //        ,[mp_advance]
    //        ,[mp_wheeling_charge]
    //        ,[mp_region_id]
    //        ,[mp_cf])
    //  VALUES
    //        ('$mp_location'
    //        ,'$mp_main_meter'
    //        ,'$mp_check_meter'
    //        ,'$mp_customer_id'
    //        ,'$time'
    //        ,'10'
    //        ,'1'
    //        ,'1'
    //        ,'1'
    //        ,'1'); ";
    //     }

    //     echo $v;

            //===================================================

         $db = new Db();
        
        $k = 231229;//last reading id

        for($kk=1; $kk<=9; $kk++)
{
  
    $date = strtotime('2023-'.str_pad($kk,2,0, STR_PAD_LEFT).'-15');
    //1694725200
       $select = $db->select("SELECT * FROM r_reading WHERE rea_date='$date' AND rea_cus_id='20190000'");
       foreach ($select[0] as $r) {
           extract($r);
           // $reading_id = 27796+$k;
           $k++;

       $db1 = new Db();
       $select1 = $db1->select("SELECT * FROM r_rate WHERE rate_cus_id='2019' AND rate_reading_id='$rea_id'");
       

        foreach($select1[0] as $row){
            extract($row);

             $x = new Db();
             $mp_location = $this->rgf("metering_point", $rea_mp_id, "mp_id", "mp_location");
            $sl = "SELECT mp_id as mpid FROM metering_point WHERE mp_location = '$mp_location' AND mp_customer_id = '2036'";
            $sel = $x->select($sl);
           // print_r($select);
            extract($sel[0][0]);

           
            $mp_customer_id = 2036;
            $time = time();

            $v .= "INSERT INTO [dbo].[r_reading]
           ([rea_date]
           ,[rea_cus_id]
           ,[rea_added_by]
           ,[rea_date_added]
           ,[rea_mp_id]
           )
     VALUES
           ('$rea_date'
           ,'2036'
           ,'10'
           ,'$time'
           ,'$mpid'); ";

    // $x = new Db();
    // $x->query($v);
    //echo $v;





        }

    //     $x = new Db();
    //     $x->query($v);
 }
  }
  //echo $v;

       
   $v = "";

    //===================================================    
//echo '<br/>xxxxx<br/>';

for($kk=1; $kk<=9; $kk++)
{
  $date = strtotime('2023-'.str_pad($kk,2,0, STR_PAD_LEFT).'-15');
    //===================================================
       $select = $db->select("SELECT * FROM r_reading WHERE rea_date='$date' AND rea_cus_id='2019000'");
       foreach ($select[0] as $r) {
           extract($r);
           // $reading_id = 27796+$k;
           $k++;

       $db1 = new Db();
       $select1 = $db1->select("SELECT * FROM r_rate WHERE rate_cus_id='2019' AND rate_reading_id='$rea_id'");
           //$v = "";

        foreach($select1[0] as $row){
            extract($row);
           
            $mp_customer_id = 2036;
            $time = time();

               $x = new Db();
             $mp_location = $this->rgf("metering_point", $rea_mp_id, "mp_id", "mp_location");
            $sl = "SELECT mp_id as mpid FROM metering_point WHERE mp_location = '$mp_location' AND mp_customer_id = '2036'";
            $sel = $x->select($sl);
           // print_r($select);
            extract($sel[0][0]);

         
    $x = new Db();
   $sl = "SELECT rea_id as vvvvvvv FROM r_reading WHERE rea_date = '$rea_date' AND rea_cus_id = '$mp_customer_id' AND rea_mp_id = '$mpid'";
    $sel = $x->select($sl);
   // print_r($select);
    extract($sel[0][0]);

            $v .= "INSERT INTO [dbo].[r_rate]
           ([rate_cus_id]
           ,[rate_import_wh_1]
           ,[rate_import_wh_2]
           ,[rate_import_wh_3]
           ,[rate_date_added]
           ,[rate_export_wh_5]
           ,[rate_added_by]
           ,[rate_reading_id]
            ,[rate_label]
            ,[rate_wc_sfe]
            ,[rate_wc_sfi]
            ,[rate_wc_lfe]
            ,[rate_wc_lfi]
            ,[rate_payable_imports]
            ,[rate_payable_exports]
            ,[rate_tlf]
            ,[rate_tlfs]
            ,[rate_sfi]
            ,[rate_sfe]
            ,[rate_lfe]
            ,[rate_lfi]
            ,[rate_narration]
           )
     VALUES
           ('$mp_customer_id'
           ,'$rate_export_wh_4'
           ,'$rate_export_wh_5'
           ,'$rate_export_wh_6'
           ,'$time'
           ,'0.0'
           ,'10'
         ,'$vvvvvvv'
        ,'$rate_label'
        ,'$rate_wc_sfe'
        ,'$rate_wc_sfi'
        ,'$rate_wc_lfe'
        ,'$rate_wc_lfi'
        ,'$rate_payable_imports'
        ,'$rate_payable_exports'
        ,'$rate_tlf'
        ,'$rate_tlfs'
        ,'$rate_sfi'
        ,'$rate_sfe'
        ,'$rate_lfe'
        ,'$rate_lfi'
        ,'$rate_narration'
     ); ";
        }

        // "rate_label"=>$rate_label,
        // "rate_wc_sfe"=>$rate_wc_sfe,
        // "rate_wc_sfi"=>$rate_wc_sfi,
        // "rate_wc_lfe"=>$rate_wc_lfe,
        // "rate_wc_lfi"=>$rate_wc_lfi,
        // "rate_payable_imports"=>$rate_payable_imports,
        // "rate_payable_exports"=>$rate_payable_exports,
        // "rate_tlf"=>$rate_tlf,
        // "rate_tlfs"=>$rate_tlfs,

    //     $x = new Db();
    //     $x->query($v);

     

       
    }

}

//echo $v;

    //===================================================
// $v = "";
// $db = new Db();
// $sql = "SELECT * FROM r_rate , r_reading where rea_cus_id = 2036 AND rea_id = rate_reading_id";
// $select = $db->select($sql);
// foreach($select[0] as $row){
//     extract($row); 
//     $mp_location = $this->rgf("metering_point", $rea_mp_id, "mp_id", "mp_location");
//     $v .= "UPDATE r_rate SET rate_sfe = '([I@".$rea_mp_id."])', rate_sfi = '', rate_lfi='([Imports@".$mp_location."])', rate_lfe = '', rate_wc_lfi = '', rate_wc_lfe = '', rate_wc_sfi = '' , rate_wc_sfe = '' WHERE rate_id = '$rate_id'; ";
// }

//echo $v;

$v = "";

$db = new Db();
$sql = "SELECT * FROM metering_point WHERE mp_customer_id = '2019'";
$select = $db->select($sql);
foreach($select[0] as $row){
    extract($row);
    $mpid = $this->rgf("metering_point", $mp_location.' - BF', 'mp_location', 'mp_id');

    $v .= "UPDATE metering_point SET mp_region_id = '$mp_region_id' WHERE mp_id = '$mpid' ";
}

//echo $v;
    
	?>
	
	<div class="block-header">
                <h2>DASHBOARD</h2>
            </div>

            <!-- Widgets -->
            <div class="row clearfix">
                
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="info-box bg-cyan hover-expand-effect">
                        <div class="icon">
                            <i class="fa fa-fx fa-plug"></i>
                        </div>
                        <?php 

                        $x = new Db();
                        $ro = $x->select("SELECT * FROM customer");
                        $total = $x->num_rows();
                        ?>
                        <div class="content">
                            <div class="text">Energy Customers</div>
                            <div class="number count-to" data-from="0" data-to="<?php echo $total; ?>" data-speed="1000" data-fresh-interval="20"></div>
                        </div>
                    </div>
                </div>
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="info-box bg-light-green hover-expand-effect">
                        <div class="icon">
                            <i class="fa fa-fx fa-wifi"></i>
                        </div>
                        <?php 
                        $x = new Db();
                        $ro = $x->select("SELECT * FROM of_customer");
                        $total = $x->num_rows();
                        ?>
                        <div class="content">
                            <div class="text">Optic Fiber Customers</div>
                            <div class="number count-to" data-from="0" data-to="<?php echo $total; ?>" data-speed="1000" data-fresh-interval="20"></div>
                        </div>
                    </div>
                </div>
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="info-box bg-orange hover-expand-effect">
                        <div class="icon">
							<i class="fa fa-fx fa-users"></i>
                        </div>
                        <?php 
                        $x = new Db();
                        $ro = $x->select("SELECT * FROM sysuser");
                        $total = $x->num_rows();
                        ?>
                        <div class="content">
                            <div class="text">System Users</div>
                            <div class="number count-to" data-from="0" data-to="<?php echo $total; ?>" data-speed="1000" data-fresh-interval="20"></div>
                        </div>
                    </div>
                </div>
            </div>
            <!-- #END# Widgets -->
        </div>
    <?php 
    }

    public function x(){
    ?>

            <div class="col-md-12">
                <?php 
                
                
                $tot_tis = 0;
                $tot_tis2 = 0;
                $customers = array();

                $date = strtotime("-1 month", time());
                $year_now = date('Y', $date);
                $month_now = date('m', $date);

                $date = strtotime("-1 month", $date);
                $past_year = date('Y', $date);
                $past_month = date('m', $date);

                $t = new Services();
                $total_vat_x = array();
                $total_vat_x_past = array();
                $customers = array();
                $db = new Db();
                $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
                foreach($select[0] as $row){
                    extract($row);
                    $customers[] = $customer_short_name;
                }

                $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
                foreach($select[0] as $row){
                    extract($row);
                    $tis = $t->invoiceTotalSummary($customer_id, $year_now, $month_now);
                    $total_vat_x[] = $tis[1];
                }

                $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
                foreach($select[0] as $row){
                    extract($row);
                    $tis = $t->invoiceTotalSummary($customer_id, $past_year, $past_month);
                    $total_vat_x_past[] = $tis[1];
                }

                for($x=0; $x<count($total_vat_x); $x++){
                    $total_vat_x[$x] = round($total_vat_x[$x]/1000000000.0, 2);
                }
                for($x=0; $x<count($total_vat_x_past); $x++){
                    $total_vat_x_past[$x] = round($total_vat_x_past[$x]/1000000000, 2);
                }
                
                echo '<div class="col-lg-12">';
                echo '<div style="width:75%;">';
                echo '<canvas id="canvas"></canvas>';
                echo '</div>';
                echo '</div>';
                
                ?>
                <script src="<?php display_url(); ?>js/Chart.min.js.js"></script>
                <script src="<?php display_url(); ?>js/utils.js"></script>

                <script>
                    var MONTHS = [<?php echo "'".implode("','", $customers)."'";?>];
                    var config = {
                        type: 'line',
                        data: {
                            labels: [<?php echo "'".implode("','", $customers)."'"; ?>],
                            datasets: [{
                                label: '<?php echo $year_now." ".month_name($month_now); ?>',
                                backgroundColor: window.chartColors.red,
                                borderColor: window.chartColors.red,
                                data: [
                                    <?php                                        
                                        echo implode(' , ', $total_vat_x);
                                    ?>
                                ],
                                fill: false,
                            }, {
                                label: '<?php echo $past_year.' '.month_name($past_month); ?>',
                                fill: false,
                                backgroundColor: window.chartColors.blue,
                                borderColor: window.chartColors.blue,
                                data: [
                                    <?php
                                        echo implode(' , ', $total_vat_x_past);
                                    ?>
                                ],
                            }]
                        },
                        options: {
                            responsive: true,
                            title: {
                                display: true,
                                text: '<?php echo 'Total Energy Sales for '.$year_now.' '.month_name($month_now).' Vs '.$past_year.' '.month_name($past_month); ?>'
                            },
                            hover: {
                                mode: 'nearest',
                                intersect: true
                            },
                            scales: {
                                xAxes: [{
                                    display: true,
                                    scaleLabel: {
                                        display: true,
                                        labelString: 'Customers'
                                    }
                                }],
                                yAxes: [{
                                    display: true,
                                    scaleLabel: {
                                        display: true,
                                        labelString: 'Amount (Billion Shs)'
                                    }
                                }]
                            }
                        }
                    };

                    window.onload = function() {
                        var ctx = document.getElementById('canvas').getContext('2d');
                        window.myLine = new Chart(ctx, config);
                    };
                    
                </script>
            </div>
            <div class="col-md-6">asd
                <?php

                ?>
            </div>

	<?php
    // $to = "josemusiitwa@gmail.com";
    // $subject = "Optic Fibre Invocie(s)";
    // $message = ''.$this->emailNotify();
    // Feedback::sendmail($to,$subject,$message,$name);
	}

}