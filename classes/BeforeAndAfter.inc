<?php
class BeforeAndAfter{
	//public $user_id = $_SESSION['USER_ID'];
	
	public $show1 = "Active";
	public $show2 = "Inactive";
	
	public $show11 = "Active";
	public $show22 = "Inactive";

	public function __call($name, $args){


		//run code before
		$method = "{$name}Action";
		if(method_exists($this, $method)){
			if($this->before() !== false){
				
				call_user_func_array([$this, $method],$args);
				$this->after();
			}
		}else{
			echo $method.' Method doesnot exists';
		}
		
	}

	public function total_imp_exp($id){
		$x = array();
		if($id){
			$db = new Db();
			$sql = "SELECT tisu, tipu, tiou, tiu, tisa, tipa, tioa, tia, tesu, tepu, teou, teu, tesa, tepa, teoa, tea, rate_advance_1 ,rate_advance_2 ,rate_advance_3 ,rate_advance_4 ,rate_advance_5 ,rate_advance_6 FROM r_rate WHERE rate_id = '$id'";
			$select = $db->select($sql);
			if($db->num_rows()){
				extract($select[0][0]);
			}
		}

		$x['tisu'] = $tisu;
		$x['tipu'] = $tipu;
		$x['tiou'] = $tiou;
		$x['tiu'] = $tiu;
		$x['tisa'] = $tisa;
		$x['tipa'] = $tipa;
		$x['tioa'] = $tioa;
		$x['tia'] = $tia;
		
		$x['tesu'] = $tesu;
		$x['tepu'] = $tepu;
		$x['teou'] = $teou;
		$x['teu'] = $teu;
		$x['tesa'] = $tesa;
		$x['tepa'] = $tepa;
		$x['teoa'] = $teoa;
		$x['tea'] = $tea;

		return $x;
	}

	public function monthTariff($year, $month){
		$db = new Db();
		$sql = "SELECT * FROM kplc_tariff_schedule WHERE ts_year = '$year' AND ts_month = '$month'";
		$select = $db->select($sql);
		extract($select[0][0]);
		$tt = ($ts_fixed_component+$ts_escalatable_component*$ts_cpim_3/$ts_cpib);
		$tt = (is_nan($tt))?"":number_format($tt,10);

		return $tt;

	}

	public function assignInvoiceNumber($year, $month, $customer_id){

		$time = strtotime(date($year.'-'.$month.'-15'));
		$dm = date('y', $time).date('m', $time);
		$suffix = $dm;

		$id_number = 0;
		$customers = array();
        $db = new Db();
        $j = new Db();
        $select = $db->select("SELECT customer_id FROM customer WHERE customer_id = '$customer_id' ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $this->lockReadings($customer_id, $year, $month);
            $c = $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");

            $sel = $j->select("SELECT TOP 1 id_number FROM invoice_date WHERE id_customer_id = '$customer_id' ORDER BY id_number DESC");
            if($j->num_rows()){
            	extract($sel[0][0]);            	
		    }
            $f = (int)(substr($id_number, 4))+1;
			$number = $suffix.str_pad($f, 3, "0", STR_PAD_LEFT);

			$sel = $j->select("SELECT id_id FROM invoice_date WHERE id_number = '$number' AND id_customer_id = '$customer_id'");

			$db = new Db();
			$sql = "SELECT id_id FROM invoice_date WHERE id_year = '$year' AND id_month = '$month' AND id_customer_id = '$customer_id'";
			$select = $db->select($sql);

            if(!$db->num_rows()){
            	$insert = $j->insert("invoice_date", [
            		"id_number"=>$number,
            		"id_year"=>$year,
            		"id_month"=>$month,
            		"id_date_added"=>time(),
            		"id_added_by"=>user_id(),
            		"id_customer"=>$c,
					"id_customer_id"=>$customer_id,
            	]);           	
		    }

		    echo $j->error();

        }
	}


	public function lockReadings($customer_id, $year, $month){
		$db = new Db();
		
		$month -= 1;
		
		if((int)$month == 1){
			$year = $year-1;
			$month = 12;
		}else{
			$month - 1;
		}
		
		$select = $db->select("SELECT lock_id FROM lock WHERE lock_year = '$year' AND lock_month = '$month' AND lock_customer_id = '$customer_id'");
		if($db->num_rows()){
			extract($select[0][0]);
			$insert = $db->update("lock", [
				"lock_status"=>1,	
				"lock_year"=>$year,	
				"lock_month"=>$month,	
				"lock_customer_id"=>$customer_id,	
			],["lock_id"=>$lock_id]);
		}else{
			$insert = $db->insert("lock", [
				"lock_status"=>1,	
				"lock_year"=>$year,	
				"lock_month"=>$month,	
				"lock_customer_id"=>$customer_id,	
			]);
		}

		
	}

	public function formulaDisplay1(){}	
	public function formulaDisplay($cus_id, $metering_point, $year, $month, $credit_note=0){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        if($credit_note)
	        	$sel = $db->select("SELECT TOP 1 n_rea_id, n_rea_date FROM r_reading_cn WHERE n_rea_cus_id = $cus_id AND n_rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        else
	        	$sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");

	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

	        if($credit_note)
	        	$sel = $db->select("SELECT n_rea_id as x, n_rea_date as y FROM r_reading_cn WHERE n_rea_cus_id = $cus_id  AND n_rea_mp_id = '$metering_point' AND rea_date = '$date'");
	        else
	        	$sel = $db->select("SELECT rea_id as x, rea_date as y FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point' AND rea_date = '$date'");
	        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
	        foreach($sel[0] as $row){
	            extract($row);
	            //if($year == date('Y', $y) && $month == date('m', $y) )
	            {
	                $rea_id = $x;
	                $rea_date = $y;
	                //echo 'reached';
	            }
	        }
	    }

	    $db = new Db();

	    if($credit_note)
	    	$sel = $db->select("SELECT TOP 1 n_rate_narration as rate_narration, n_rate_lfi as rate_lfi, n_rate_lfe as rate_lfe, n_rate_sfi as rate_sfi, n_rate_sfe as rate_sfe, n_rate_label as rate_label FROM r_rate_cn WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id'");
	    else
	    	$sel = $db->select("SELECT TOP 1 rate_narration, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");

	    extract($sel[0][0]);

	    if($credit_note)
	    	echo '<p class="text-danger" style="padding:0; margin:0; margin-bottom:10px;"><span class="text-primary text-bold"><div style="text-align:center;font-weight:bold;">'.$this->rgf("metering_point", $metering_point, "mp_id", "mp_location").'</span> ADVANCE FORMULAE (KWh)</div><span style="margin-top:10px;"><a href="'.return_url().'formulae-builder/formulae/'.$cus_id.'/'.$year.'/'.$month.'/'.$mp_id.'/credit-note" style="color:blue !important;">View In Formula Builder</a></span></p>';
	    else
	    	echo '<p class="text-danger" style="padding:0; margin:0; margin-bottom:10px;"><span class="text-primary text-bold"><div style="text-align:center;font-weight:bold;">'.$this->rgf("metering_point", $metering_point, "mp_id", "mp_location").'</span> ADVANCE FORMULAE (KWh)</div><span style="margin-top:10px;"><a href="'.return_url().'formulae-builder/formulae/'.$cus_id.'/'.$year.'/'.$month.'/'.$mp_id.'" style="color:blue !important;">View In Formula Builder</a></span></p>';

	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){

		    echo '<div class="row">';

		    // echo $rate_sfi;
		    // echo '<br/>';
		    // echo $rate_sfe;
		    echo '<div class="col-md-6">';
		    echo '<b>Imports: </b><br/>'.$rate_lfi.'<br/>';
		    $r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfe);
		    echo '</div>';

		    echo '<div class="col-md-6">';
		    echo '<b>Exports: </b><br/>'.$rate_lfe.'<br/>';
		    $r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfi);
		    echo '</div>';

		    echo '<div class="col-md-12">';
		    echo '<div style="margin-top:10px;"><b>Narration: </b><br/>'.$rate_narration.'</div>';
		    echo '</div>';

		    echo '</div>';
		}else{
			echo "<center>No Formulae</center>";
		}

		echo $db->error();
		    echo '';
	}
	
	public function formulaDisplayFx($cus_id, $metering_point, $year, $month){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

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

	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rate_narration, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);

	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){
		    echo '<b>Imports: </b>'.$rate_lfi.'&nbsp;&nbsp;';
		    echo '<br/><b>Exports: </b>'.$rate_lfe.'';
		    if($rate_narration)
		    	echo '<br/><b>Narration: </b>'.$rate_narration.'';
		}else{
			echo "No Formulae";
		}

	}
	public function formulaDisplayFx2($cus_id, $metering_point, $year, $month){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

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

	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rate_narration, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);

	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){	    	
		    $r1 = $this->get_readings21($customer, $metering_point, $month, $year, $rate_sfe);
		    $r2 = $this->get_readings21($customer, $metering_point, $month, $year, $rate_sfi);
		}else{
			echo "No Formulae";
		}

	}
	
	public function formulaDisplay3($cus_id, $metering_point, $year, $month){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

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

	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rate_narration,rate_label, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);

	   // echo '<p class="text-danger" style="padding:0; margin:0; margin-bottom:10px;"><span class="text-primary text-bold"><div style="text-align:center;font-weight:bold;">'.$this->rgf("metering_point", $metering_point, "mp_id", "mp_location").'</span> ADVANCE FORMULAE (KWh)</div><span style="margin-top:10px;"><a href="'.return_url().'formulae-builder/formulae/'.$cus_id.'/'.$year.'/'.$month.'/'.$mp_id.'" style="color:blue !important;">View In Formula Builder</a></span></p>';

	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){

		    echo '<div class="row">';

		    // echo $rate_sfi;
		    // echo '<br/>';
		    // echo $rate_sfe;
		    echo '<div class="col-md-6" style="float:left; width:49%;">';
		    echo '<b>Imports: </b><br/>'.$rate_lfi.'<br/>';
		    $r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfe);
		    echo '</div>';

		    echo '<div class="col-md-6" style="float:right; width:49%;">';
		    echo '<b>Exports: </b><br/>'.$rate_lfe.'<br/>';
		    $r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfi);
		    echo '</div>';

		    echo '<div class="col-md-12">';
		    if($rate_label)
		    	echo '<div style="margin-top:10px;"><b>New Label: </b>'.$rate_label.'</div>';
			if($rate_label)
		    	echo '<div style="margin-top:10px;"><b>Narration: </b><br/>'.$rate_narration.'</div>';
		    echo '</div>';

		    echo '</div>';
		}else{
			echo "<center>No Formulae</center>";
		}


		    echo '';
	}

	public function checkDont($cus_id, $metering_point, $year, $month){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

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

	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rate_narration,rate_label, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);


	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){

		    if($rate_lfi == "([DONOT_DISPLAY])" && $rate_lfi == "([DONOT_DISPLAY])"){
		    	return 0;
			}
		}else{
			return 0;
		}

		return 1;

	}
	
	
	public function formulaDisplay4($cus_id, $metering_point, $year, $month){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

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

	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rate_narration,rate_label, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);

	   // echo '<p class="text-danger" style="padding:0; margin:0; margin-bottom:10px;"><span class="text-primary text-bold"><div style="text-align:center;font-weight:bold;">'.$this->rgf("metering_point", $metering_point, "mp_id", "mp_location").'</span> ADVANCE FORMULAE (KWh)</div><span style="margin-top:10px;"><a href="'.return_url().'formulae-builder/formulae/'.$cus_id.'/'.$year.'/'.$month.'/'.$mp_id.'" style="color:blue !important;">View In Formula Builder</a></span></p>';

	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){

		    echo '<div style="width:100%;float:left;padding:0 10px;">';

		    // echo $rate_sfi;
		    // echo '<br/>';
		    // echo $rate_sfe;
		    echo '<div style="float:left; width:49%;">';
		    if($rate_lfi != "([DONOT_DISPLAY])"){
		    	echo '<b>Imports: </b><br/>'.$rate_lfi.'<br/>';
		    	$r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfe);
			}
		    echo '</div>';

		    echo '<div style="float:right; width:49%;">';
		    if($rate_lfi != "([DONOT_DISPLAY])"){
		    	echo '<b>Exports: </b><br/>'.$rate_lfe.'<br/>';
		    	$r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfi);
			}
		    echo '</div>';

		    echo '</div>';
		}else{
			echo "<center>No Formulae</center>";
		}


		    echo '';
	}
	
	public function formulaDisplay2($cus_id, $metering_point, $year, $month){
		$db = new Db();
		$date = strtotime($year.'-'.$month.'-15');

		if(empty($date)){
        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

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

	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rate_narration,rate_label, rate_lfi, rate_lfe, rate_sfi, rate_sfe, rate_label, rate_wc_lfi, rate_wc_lfe FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);

	   // echo '<p class="text-danger" style="padding:0; margin:0; margin-bottom:10px;"><span class="text-primary text-bold"><div style="text-align:center;font-weight:bold;">'.$this->rgf("metering_point", $metering_point, "mp_id", "mp_location").'</span> ADVANCE FORMULAE (KWh)</div><span style="margin-top:10px;"><a href="'.return_url().'formulae-builder/formulae/'.$cus_id.'/'.$year.'/'.$month.'/'.$mp_id.'" style="color:blue !important;">View In Formula Builder</a></span></p>';

	    if($rate_lfi != ""||$rate_lfe != ""||$rate_narration){

		    echo '<div class="row">';

		    // echo $rate_sfi;
		    // echo '<br/>';
		    // echo $rate_sfe;
		    echo '<div class="col-md-4">';
		    echo ''.$rate_lfi.'';
		    //$r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfe);
		    echo '</div>';

		    echo '<div class="col-md-4">';
		    echo ''.$rate_lfe.'';
		    //$r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfi);
		    echo '</div>';
		    echo '<div class="col-md-4">';
		    echo ''.$rate_wc_lfi.'';
		    //$r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfi);
		    echo '</div>';
		    echo '<div class="col-md-4">';
		    echo ''.$rate_wc_lfe.'';
		    //$r = $this->get_readings2($customer, $metering_point, $month, $year, $rate_sfi);
		    echo '</div>';

		    echo '</div>';
		}else{
			echo "<center>No Formulae</center>";
		}


		    echo '';
	}

	public function rate_total($cus_id, $date, $type="i", $credit_note = 0){
		$db = new Db();

		if($credit_note == 1)
			$sql = "SELECT n_tiou as tiou, n_tipu as tipu, n_tisu as tisu, n_tiu as tiu, n_teou as teou, n_tepu as tepu, n_tesu as tesu, n_teu as teu FROM r_reading_cn, r_rate_cn WHERE n_rea_cus_id = '$cus_id' AND n_rate_reading_id = n_rea_id AND n_rea_date = '$date'";
		else
			$sql = "SELECT tiou, tipu, tisu, tiu, teou, tepu, tesu, teu FROM r_reading, r_rate WHERE rea_cus_id = '$cus_id' AND rate_reading_id = rea_id AND rea_date = '$date'";

		$select = $db->select($sql);

		$tteou=$ttepu=$ttesu=$tteu=$ttiou=$ttipu=$ttisu=$ttiu=0;

		if($db->num_rows()){
			foreach($select[0] as $row){
				extract($row);
				$ttisu += $tisu;
				$ttiou += $tiou;
				$ttipu += $tipu;
				$ttiu += $tiu;
				$ttesu += $tesu;
				$tteou += $teou;
				$ttepu += $tepu;
				$tteu += $teu;
			}
		}

		if($type != "i")
			return array($ttesu, $ttepu, $tteou, $tteu);
		else
			return array($ttisu, $ttipu, $ttiou, $ttiu);
	}

	public function rate_calculate($cus_id, $date="", $metering_point=""){
	    
	    $db = new Db();
	    global $con;
	    if(empty($date)){
	        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

	        $start_date = strtotime($year.'-'.$month.'-01 12:00:00 am');
	        $end_date = $year.'-'.$month.'-'.date('t', $start_date).' 11:59:59 pm';
	        $end_date = strtotime($end_date);

	        $db = new Db();
	        $sql = "SELECT rea_id as x, rea_date as y FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point'  AND rea_date >= '$start_date' AND rea_date <= '$end_date'";
	        $sel = $db->select($sql);
	        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
	        if($db->num_rows()){
	        foreach($sel[0] as $row){
	            extract($row);
	            if($year == date('Y', $y) && $month == date('m', $y) ){
	                $rea_id = $rea_idlast = $x;
	                $rea_date = $y;
	                //echo 'reached';
	            }
	        }
	        }
	    }
	    
	    $sel = $db->select("SELECT TOP 1 rate_import_wh_1 as rat1, rate_import_wh_2 as rat2, rate_import_wh_3 as rat3,rate_export_wh_4 as rat4,rate_export_wh_5 as rat5,rate_export_wh_6 as rat6, rate_cus_id, rate_advance_1 as ais, rate_advance_2 as aip, rate_advance_3 as aio, rate_advance_4 as aes, rate_advance_5 as aep, rate_advance_6 as aeo, rate_swap, rate_label as label, rate_sfi as sfi, rate_sfe as sfe, rate_wheeling_charge as wheeling_charge, rate_wc_sfe as wc_sfe, rate_wc_sfi as wc_sfi, rate_wc_lfe as wc_lfe, rate_wc_lfi as wc_lfi FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);

	    //print_r($select);

	    //$rea_date = date('m', ($rea_date));
	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 rea_id FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point' AND rea_date < $rea_date ORDER BY rea_date DESC");  
	    extract($sel[0][0]);
	    
	    //echo $rea_date;

	    if($db->num_rows()){      
	        //selecting rates
	        $db = new Db();

	        $sql = "SELECT TOP 1 n_rate_import_wh_1 as rate1, n_rate_import_wh_2 as rate2, n_rate_import_wh_3 as rate3,n_rate_export_wh_4 as rate4, n_rate_export_wh_5 as rate5,n_rate_export_wh_6 as rate6, n_rate_cus_id FROM r_rate_cn, r_reading_cn WHERE n_rea_mp_id = '$metering_point' AND n_rate_cus_id = '$cus_id' AND n_rea_id = n_rate_reading_id AND n_rea_date < $rea_date AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )";

			$sel = $db->select($sql);

			if(!$db->num_rows())
			{
	        	$sel = $db->select("SELECT TOP 1 rate_import_wh_1 as rate1, rate_import_wh_2 as rate2, rate_import_wh_3 as rate3,rate_export_wh_4 as rate4, rate_export_wh_5 as rate5,rate_export_wh_6 as rate6, rate_cus_id FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	        }

	        extract($sel[0][0]); 
	    }

	    if(1){
	        $r1 = ceil(@$rat1 - @$rate1);
	        $r2 = ceil(@$rat2 - @$rate2);
	        $r3 = ceil(@$rat3 - @$rate3);
	        $r4 = ceil(@$rat4 - @$rate4);
	        $r5 = ceil(@$rat5 - @$rate5);
	        $r6 = ceil(@$rat6 - @$rate6);
	    }else{
	        $r1 = ceil(@$rat4 - @$rate4);
	        $r2 = ceil(@$rat5 - @$rate5);
	        $r3 = ceil(@$rat6 - @$rate6);
	        $r4 = ceil(@$rat1 - @$rate1);
	        $r5 = ceil(@$rat2 - @$rate2);
	        $r6 = ceil(@$rat3 - @$rate3);
	    }
	    if($ais !="" || $aip != "" || $aio != "")
	        $b=1;
	    else
	        $b=0;
	    
	    if(($ais != "")){ 
	        $r1 = $ais;
	    }
	    if(($aip != "")){ 
	        $r2 = $aip;
	    }
	    if(($aio != "")){ 
	        $r3 = $aio;        
	    }
	    if(($aes != "")){ 
	        $r4 = $aes;
	    }
	    if(($aep != "")){ 
	        $r5 = $aep;
	    }
	    if(($aeo != "")){ 
	        $r6 = $aeo;
	    }
	    
	    if($rate_swap){
	        $j1 = $r4;
	        $j2 = $r5;
	        $j3 = $r6;
	        $j4 = $r1;
	        $j5 = $r2;
	        $j6 = $r3;
	        
	        $r1 = $j1; $r2=$j2; $r3=$j3; $r4=$j4; $r5=$j5; $r6=$j6;
	    }
	    
	    $x = ($r1-$r4);
	    $y = ($r2-$r5);
	    $z = ($r3-$r6);

	    //                  0   1   2        3        4    5   6       7       8  9  10      11
	    $to_return = array($r1,$r2,$r3,($r1+$r2+$r3),$r4,$r5,$r6,($r4+$r5+$r6),$x,$y,$z, ($x+$y+$z), 
	        $rate1, // 12
	        $rate2, // 13
	        $rate3, // 14
	        $rate4, // 15
	        $rate5, // 16
	        $rate6,  // 17
	        $rate_swap,//18
	        array(//19
	            $rat1,
	            $rat2,
	            $rat3,
	            $rat4,
	            $rat5,
	            $rat6,
	        ),
	        array(//20
	            $rate1,
	            $rate2,
	            $rate3,
	            $rate4,
	            $rate5,
	            $rate6,
	        ),
	        $label,//21
	        $sfi,//22
	        $sfe,//23
	        $wheeling_charge,//24
	        $b,//25
	        $rea_idlast,//26
	        $wc_sfi,//27
	        $wc_sfe,//28
	        $wc_lfi,//29
	        $wc_lfe,//30
	    );
	    
	    return $to_return;

	    
	}
	public function rate_calculate_cn($cus_id, $date="", $metering_point=""){
	        
	    $db = new Db();
	    global $con;
	    if(empty($date)){
	        //sdafaf
	        $sel = $db->select("SELECT TOP 1 n_rea_id as rea_id, n_rea_date as rea_date FROM r_reading_cn WHERE n_rea_cus_id = $cus_id AND n_rea_mp_id = '$metering_point' ORDER BY n_rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

	        $start_date = strtotime($year.'-'.$month.'-01 12:00:00 am');
	        $end_date = $year.'-'.$month.'-'.date('t', $start_date).' 11:59:59 pm';
	        $end_date = strtotime($end_date);

	        $db = new Db();
	        $sql = "SELECT n_rea_id as x, n_rea_date as y FROM r_reading_cn, r_rate_cn WHERE n_rate_reading_id = n_rea_id AND n_rea_cus_id = $cus_id  AND n_rea_mp_id = '$metering_point'  AND n_rea_date >= '$start_date' AND n_rea_date <= '$end_date'  AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )";
	        $sel = $db->select($sql);
	        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
	        if($db->num_rows()){
	        foreach($sel[0] as $row){
	            extract($row);
	            if($year == date('Y', $y) && $month == date('m', $y) ){
	                $rea_id = $rea_idlast = $x;
	                $rea_date = $y;
	                //echo 'reached';
	            }
	        }
	        }
	    }

	    $sql = "SELECT TOP 1 n_rate_import_wh_1 as rat1, n_rate_import_wh_2 as rat2, n_rate_import_wh_3 as rat3,n_rate_export_wh_4 as rat4,n_rate_export_wh_5 as rat5,n_rate_export_wh_6 as rat6, n_rate_cus_id, n_rate_advance_1 as ais, n_rate_advance_2 as aip, n_rate_advance_3 as aio, n_rate_advance_4 as aes, n_rate_advance_5 as aep, n_rate_advance_6 as aeo, n_rate_swap, n_rate_label as label, n_rate_sfi as sfi, n_rate_sfe as sfe, n_rate_wheeling_charge as wheeling_charge, n_rate_wc_sfe as wc_sfe, n_rate_wc_sfi as wc_sfi, n_rate_wc_lfe as wc_lfe, n_rate_wc_lfi as wc_lfi FROM r_rate_cn WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id' AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )";
	    $sel = $db->select($sql);
	    extract($sel[0][0]);

	    //print_r($select);

	    //$rea_date = date('m', ($rea_date));
	    $db = new Db();
	    $sel = $db->select("SELECT TOP 1 n_rea_id as rea_id FROM r_reading_cn_previous WHERE n_rea_cus_id = $cus_id  AND n_rea_mp_id = '$metering_point' AND n_rea_date = $rea_date ORDER BY n_rea_date DESC");  
	    extract($sel[0][0]);
	    
	    //echo $rea_date;

	    if($db->num_rows()){      
	        //selecting rates
	        $sel = $db->select("SELECT TOP 1 n_rate_import_wh_1 as rate1, n_rate_import_wh_2 as rate2, n_rate_import_wh_3 as rate3,n_rate_export_wh_4 as rate4, n_rate_export_wh_5 as rate5,n_rate_export_wh_6 as rate6, n_rate_cus_id FROM r_rate_cn_previous WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id'");
	        extract($sel[0][0]); 
	    }

	    if(1){
	        $r1 = ceil(@$rat1 - @$rate1);
	        $r2 = ceil(@$rat2 - @$rate2);
	        $r3 = ceil(@$rat3 - @$rate3);
	        $r4 = ceil(@$rat4 - @$rate4);
	        $r5 = ceil(@$rat5 - @$rate5);
	        $r6 = ceil(@$rat6 - @$rate6);
	    }else{
	        $r1 = ceil(@$rat4 - @$rate4);
	        $r2 = ceil(@$rat5 - @$rate5);
	        $r3 = ceil(@$rat6 - @$rate6);
	        $r4 = ceil(@$rat1 - @$rate1);
	        $r5 = ceil(@$rat2 - @$rate2);
	        $r6 = ceil(@$rat3 - @$rate3);
	    }
	    if($ais !="" || $aip != "" || $aio != "")
	        $b=1;
	    else
	        $b=0;
	    
	    if(($ais != "")){ 
	        $r1 = $ais;
	    }
	    if(($aip != "")){ 
	        $r2 = $aip;
	    }
	    if(($aio != "")){ 
	        $r3 = $aio;        
	    }
	    if(($aes != "")){ 
	        $r4 = $aes;
	    }
	    if(($aep != "")){ 
	        $r5 = $aep;
	    }
	    if(($aeo != "")){ 
	        $r6 = $aeo;
	    }
	    
	    if($rate_swap){
	        $j1 = $r4;
	        $j2 = $r5;
	        $j3 = $r6;
	        $j4 = $r1;
	        $j5 = $r2;
	        $j6 = $r3;
	        
	        $r1 = $j1; $r2=$j2; $r3=$j3; $r4=$j4; $r5=$j5; $r6=$j6;
	    }
	    
	    $x = ($r1-$r4);
	    $y = ($r2-$r5);
	    $z = ($r3-$r6);

	    //                  0   1   2        3        4    5   6       7       8  9  10      11
	    $to_return = array($r1,$r2,$r3,($r1+$r2+$r3),$r4,$r5,$r6,($r4+$r5+$r6),$x,$y,$z, ($x+$y+$z), 
	        $rate1, // 12
	        $rate2, // 13
	        $rate3, // 14
	        $rate4, // 15
	        $rate5, // 16
	        $rate6,  // 17
	        $rate_swap,//18
	        array(//19
	            $rat1,
	            $rat2,
	            $rat3,
	            $rat4,
	            $rat5,
	            $rat6,
	        ),
	        array(//20
	            $rate1,
	            $rate2,
	            $rate3,
	            $rate4,
	            $rate5,
	            $rate6,
	        ),
	        $label,//21
	        $sfi,//22
	        $sfe,//23
	        $wheeling_charge,//24
	        $b,//25
	        $rea_idlast,//26
	        $wc_sfi,//27
	        $wc_sfe,//28
	        $wc_lfi,//29
	        $wc_lfe,//30
	    );
	    
	    return $to_return;
	    
	}
	
	public function get_readings($reading_type, $customer, $point_id, $month, $year, $credit_note = 0){

		//echo '======='.$year.'<br/>';
		$date = strtotime($year.'-'.$month.'-15');
		if($reading_type == "TI"){
			$r = $this->sumFormulae($point_id, $year, $month);
			return array('R1'=>$r[0],'R2'=>$r[1],'R3'=>$r[2],'TOTAL'=>$r[3]);
		}else if($reading_type=="TE"){
			$r = $this->sumFormulae($point_id, $year, $month);
			return array('R1'=>$r[0],'R2'=>$r[1],'R3'=>$r[2],'TOTAL'=>$r[3]);
		}

		if($credit_note)
			$r = $this->rate_calculate_cn($customer, $date, $point_id);
		else
			$r = $this->rate_calculate($customer, $date, $point_id);
		
		//echo $formula.'....';
		if($reading_type == "I"){
			return array('R1'=>$r[0],'R2'=>$r[1],'R3'=>$r[2],'TOTAL'=>$r[3]);
		}elseif($reading_type == "E"){
			return array('R1'=>$r[4],'R2'=>$r[5],'R3'=>$r[6],'TOTAL'=>$r[7]);
		}elseif($reading_type == "NI"){
			return array('R1'=>$r[0]-$r[4],'R2'=>$r[1]-$r[5],'R3'=>$r[2]-$r[6], 'TOTAL'=>$r[3]-$r[7]);
		}elseif($reading_type == "NE"){
			return array('R1'=>($r[0]-$r[4])*-1,'R2'=>($r[1]-$r[5])*-1,'R3'=>($r[2]-$r[6])*-1, 'TOTAL'=>($r[3]-$r[7])*-1);
		}
	}

	
	public function get_readings2($customer, $point_id, $month, $year, $formula, $credit_note = 0){
	
		$test = $formula;
		if(0 && (strpos($test, "([TI@") !== false || strpos($test, "([TE@") !== false)){	
				
			$customer = end(explode('@', $test));
			$customer = str_replace('])','',$customer);

			
			$g = array('R1','R2','R3','TOTAL');
			$g = array();
			$g['R1'] = (double)str_replace(',','',$test[0]);
			$g['R2'] = (double)str_replace(',','',$test[1]);
			$g['R3'] = (double)str_replace(',','',$test[2]);
			$g['TOTAL'] =  (double)$test[0] + (double)$test[1] + (double)$test[2];
			$v=0;

			$Cal = new Field_calculate();
			echo '<table border="1" cellspacing="0" style="min-width:100px; width:100%;font-size:10px;"><tr><td colspan="3" style="padding:0 2px;">';
			echo '<b>ADVANCE</b>:</b></td></tr>';
			foreach($g as $gg=>$v){
				echo '<tr>';
				echo '<td style="padding:0px 2px; "><b>A'.$gg.'</b></td>';
				echo '<td style="text-align:right;padding:0px 2px;"><b>'.($Cal->Calculate($v)/1000).'</b></td>';
				echo '<td style="text-align:right;padding:0px 2px;">'.($v/1000).'</td>';
				echo '</tr>';
			}
			echo '</table>';

		}else if(strpos($test, "([NetAdvance@(") !== false){
			//([NetAdvance@(VALUE_1)(VALUE_2)(VALUE_3)]) 
			$test = str_replace('([NetAdvance@(','',$test);
			$test = str_replace(')])','',$test);
			$test = explode(')(',$test);

			$g = array();
			$g['R1'] = (double)str_replace(',','',$test[0]);
			$g['R2'] = (double)str_replace(',','',$test[1]);
			$g['R3'] = (double)str_replace(',','',$test[2]);
			$g['TOTAL'] =  (double)$test[0] + (double)$test[1] + (double)$test[2];
			$v=0;

			$Cal = new Field_calculate();
			echo '<table border="1" cellspacing="0" style="min-width:100px; width:100%;font-size:10px;"><tr><td colspan="3" style="padding:0 2px;">';
			echo '<b>ADVANCE</b>:</b></td></tr>';
			foreach($g as $gg=>$v){
				echo '<tr>';
				echo '<td style="padding:0px 2px; "><b>A'.$gg.'</b></td>';
				echo '<td style="text-align:right;padding:0px 2px;"><b>'.($Cal->Calculate($v)).'</b></td>';
				echo '<td style="text-align:right;padding:0px 2px;">'.$v.'</td>';
				echo '</tr>';
			}
			echo '</table>';

		}else if($test != '([DONOT_DISPLAY])'){
			$signs = array();
			preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9+*-\.\/]+)?\)/',$formula,$matches,PREG_PATTERN_ORDER);
			
			preg_match_all('/\)[+\/*-]\(/',$formula,$matches2,PREG_PATTERN_ORDER);
			//echo '<pre>';print_r($matches2);echo '</pre>';
			$signs=$matches2[0];
			$all_arrays = array();
			$totals = 0;
			$m = array();
			$number_and_sign = array();
			foreach($matches[0] as $ap){
				//removing multiple
				$ab = $ap;			
				//$aa = preg_split('/[+-\/*]/', $ap);
				$ap = "@@@@".$ap."@@@@";

				$ap = str_replace('@@@@([', '', $ap);
				$ap = str_replace('])@@@@', '', $ap);
				$ap = str_replace(']*', '*', $ap);
				$ap = str_replace(']/', '/', $ap);
				$ap = str_replace(']-', '-', $ap);
				$ap = str_replace(']+', '+', $ap);
				$ap = str_replace(')', '', $ap);
				$ap = str_replace('@@@@', '', $ap);
				//echo '<br/>'.$ap = str_replace(')@@@@', '', $ap);
				//echo '<br/>'.$ap = str_replace(']', '', $ap);
				
				//echo '<br/>b ap: '.$ap;

				preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);
				//print_r($matches3);
				$nas = $matches3[0][0];
				$ap = str_replace($nas, '', $ap);
				$p = explode('@',$ap);
				$number_and_sign[$totals] = $nas;
				
				$m[$totals][0] = $ab;//formula
				$m[$totals][1] = $ap;
				$m[$totals][2] = $nas;
				$type = $p[0];
				$po = $p[1];

				$cust = $this->rgf("metering_point", $po, "mp_id", "mp_customer_id");
				$r = $this->get_readings($type, $cust, $po, $month, $year, $credit_note);

				$all_arrays[] = $r;
				$totals++;
			}
			// echo '<pre>';
			// print_r($all_arrays);
			// echo '</pre>';
			$g = array('R1','R2','R3','TOTAL');
			$formulas = array();
			$formulas_eval = array();
			foreach($g as $gg){
				$string = "";
				$string_to_eval = "";
				for($i=0; $i<$totals; $i++){
					//echo "$i $gg";
					if($m[$i][0]=='(['.$m[$i][1].'])'){
					//if(1){
						$string .= number_format($all_arrays[$i][$gg]/1000);
						$string_to_eval .= ($all_arrays[$i][$gg]/1000);
					}else{						
						$string .= number_format($all_arrays[$i][$gg]/1000).$m[$i][2];
						$string_to_eval .= ($all_arrays[$i][$gg]/1000).$m[$i][2];
					}
					if($i<$totals-1){
						$s = $signs[$i];
						$s = str_replace(')',"", $s);
						$s = str_replace('(',"", $s);
						$string .= " ".$s."&nbsp;";
						$string_to_eval .=$s;
					}
				}
				$string_to_eval = str_replace("-0", "", $string_to_eval);
				$string_to_eval = str_replace("--", "+", $string_to_eval); 
				$formulas[]= $string;
				$formulas_eval[]= $string_to_eval;
			}

			// print_r($formulas);
			$Cal = new Field_calculate();
			$v = 0;
			echo '<table border="1" cellspacing="0" style="min-width:100px; width:100%;font-size:10px;"><tr><td colspan="3" style="padding:0 2px;">';
			echo '<b>ADVANCE</b>:</b></td></tr>';
			foreach($g as $gg){
				echo '<tr>';
				echo '<td style="padding:0px 2px; "><b>A'.$gg.'</b></td>';
				echo '<td style="text-align:right;padding:0px 2px;"><b>'.number_format($Cal->Calculate($formulas_eval[$v])).'</b></td>';
				echo '<td style="text-align:right;padding:0px 2px;">'.($formulas[$v++]).'</td>';
				echo '</tr>';
			}
			echo '</table>';

		}else{
			$formula = "";
		}
		return $formula;
	}

	public function isTerminated($contract){
		$db = new Db();
		$select = $db->select("SELECT contract_t_date FROM contract WHERE contract_id = '$contract'");
		extract($select[0][0]);
		return $contract_t_date;
	}

	public function get_bill_price($generator, $year, $month, $formula, $h=0){
		$formula_to_eval = $formula;
		$db = new Db();

		$mix_amount3=$mix_amount2=$mix_amount=$ex_usd=$ex_euro=$ex_shs="<b>[Field Not Found]</b>";
		$sql = "SELECT * FROM mix_price WHERE mix_generator_id = '$generator' AND mix_year = '$year' AND mix_month = '$month'";
		$select = $db->select($sql);
		extract($select[0][0]);
		// echo '<pre>';
		// print_r($select[0]);
		// echo '</pre>';
		if($db->num_rows()){
			$select = $db->select("SELECT ex_usd, ex_euro, ex_shs FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
			extract($select[0][0]);

			if($mix_currency == "USD") $amount = $mix_amount;
			elseif($mix_currency == "EUR") $amount = $mix_amount2;
			elseif($mix_currency == "UGX") $amount = $mix_amount3;

			if($mix_amount3 != 0) echo  "<br/>([Bill_Ugx]) = ".number_format($mix_amount3/1000)."";
			if($mix_amount2 != 0) echo  "<br/>([Bill_Eur]) = ".number_format($mix_amount2/1000)."";
			if($mix_amount != 0) echo  "<br/>([Bill_Usd]) = ".number_format($mix_amount/1000)."";
			if($mix_units != 0) echo  "<br/>([Units]) = ".number_format($mix_units/1000)."";
			echo '<br/>';

			//echo '<br/><b>BILL CURRENCY: </b>'.$mix_currency.'<br/>';
			echo '<br/>'.$gname;
			echo ' = ';

			$formula_to_eval = str_replace('([Bill_Ugx])', ' '.number_format($mix_amount3/1000).' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Bill_Usd])', ' '.number_format($mix_amount/1000).' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Bill_Eur])', ' '.number_format($mix_amount2/1000).' ', $formula_to_eval);

			$formula_to_eval = str_replace('([Units])', ' '.number_format($mix_units/1000).' ', $formula_to_eval);

			if($ex_shs != "[Field Not Found]") $ex_shs = number_format($ex_shs, 2);
			if($ex_usd != "[Field Not Found]") $ex_usd = number_format($ex_usd, 2);
			if($ex_euro != "[Field Not Found]") $ex_euro = number_format($ex_euro, 2);

			$formula_to_eval = str_replace('([Ex_Ugx])', ' '.$ex_shs.' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Ex_Eur])', ' '.$ex_euro.' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Ex_Usd])', ' '.$ex_usd.' ', $formula_to_eval);
			
			echo $formula_to_eval;
			$exp = str_replace(" ", "", $formula_to_eval);
			$exp = str_replace(",", "", $exp);
			//echo '<br/>'.$exp;
		
			if(!strpos($formula_to_eval,"[Field Not Found]")){
				echo '<br/>';
				$Cal = new Field_calculate();
				echo 'Price(USD/kWh) '.' = <b>'.number_format($Cal->Calculate($exp),5).'</b>';
				echo '<br/><br/>';
				if($h==0){
					echo '<input type="button" id="save" value=" Save " name="save"/>';
					echo '<span id="saveStatus"></span>';
				}
			}else{
				echo '<br/><br/><span class="text-danger">Please Correct Found to Proceed</span>';
			}
		}else{
			echo '<br/><br/><span class="text-danger">Bill not found</span>';
		}

	}
	
	public function get_bill_price2($year, $month, $generator, $formula=""){
		$formula_to_eval = $formula;
		$db = new Db();

		$mix_amount3=$mix_amount2=$mix_amount=$ex_usd=$ex_euro=$ex_shs="<b>[Field Not Found]</b>";
		$sql = "SELECT * FROM mix_price WHERE mix_generator_id = '$generator' AND mix_year = '$year' AND mix_month = '$month'";
		$select = $db->select($sql);
		extract($select[0][0]);
		if(empty($formula_to_eval)){
			$formula_to_eval = $mix_formulae;
		}
		// echo '<pre>';
		// print_r($select[0]);
		// echo '</pre>';
		if($db->num_rows()){
			$select = $db->select("SELECT ex_usd, ex_euro, ex_shs FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
			extract($select[0][0]);

			if($mix_currency == "USD") $amount = $mix_amount;
			elseif($mix_currency == "EUR") $amount = $mix_amount2;
			elseif($mix_currency == "UGX") $amount = $mix_amount3;


			$formula_to_eval = str_replace('([Bill_Ugx])', ' '.number_format($mix_amount3).' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Bill_Usd])', ' '.number_format($mix_amount).' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Bill_Eur])', ' '.number_format($mix_amount2).' ', $formula_to_eval);

			$formula_to_eval = str_replace('([Units])', ' '.number_format($mix_units).' ', $formula_to_eval);

			if($ex_shs != "[Field Not Found]") $ex_shs = number_format($ex_shs, 2);
			if($ex_usd != "[Field Not Found]") $ex_usd = number_format($ex_usd, 2);
			if($ex_euro != "[Field Not Found]") $ex_euro = number_format($ex_euro, 2);

			$formula_to_eval = str_replace('([Ex_Ugx])', ' '.$ex_shs.' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Ex_Eur])', ' '.$ex_euro.' ', $formula_to_eval);
			$formula_to_eval = str_replace('([Ex_Usd])', ' '.$ex_usd.' ', $formula_to_eval);
			
			$exp = str_replace(" ", "", $formula_to_eval);
			$exp = str_replace(",", "", $exp);
			//echo '<br/>'.$exp;
			$v = "";

			if($mix_amount3 != 0) $shs = $mix_amount3;
			if($mix_amount2 != 0) $euro = ($mix_amount2);
			if($mix_amount != 0) $usd = ($mix_amount);

			if($mix_units != 0)  $units = number_format($mix_units);
		
			if(!strpos($formula_to_eval,"[Field Not Found]")){
				$Cal = new Field_calculate();
				$x = $Cal->Calculate($exp);
				if(is_nan($x)){
					$x = 0;
				}
				return array(
					"price"=>number_format($x, 20),
					"bill"=>$v,
					"formulae"=>$mix_formulae,
					"units"=>$units,
					"usd"=>$usd,
					"euro"=>$euro,
					"shs"=>$shs,
				);
			}else{
				return array(
					"price"=>0.0000,
					"bill"=>$v,
					"formulae"=>$mix_formulae,
					"units"=>$units,
					"usd"=>$usd,
					"euro"=>$euro,
					"shs"=>$shs,
				);
			}
		}else{
			return array(
				"price"=>0.000,
				"bill"=>$v,
				"formulae"=>$mix_formulae,
				"units"=>$units,
				"usd"=>$usd,
				"euro"=>$euro,
				"shs"=>$shs,
			);
		}

	}
	

	public function get_advance($customer, $point_id, $month, $year, $formula, $credit_note = 0){
		//echo $customer.' => '.$point_id.' => '.$month.' => '.$year.' => '.$formula;
		$vvv=array();
		$test = $formula;
		if(strpos($test, "([NetAdvance@(") !== false){
			//([NetAdvance@(VALUE_1)(VALUE_2)(VALUE_3)]) 
			$test = str_replace('([NetAdvance@(','',$test);
			$test = str_replace(')])','',$test);
			$test = explode(')(',$test);

			$g = array();
			$g['AR1'] = (double)str_replace(',','',$test[0]);
			$g['AR2'] = (double)str_replace(',','',$test[1]);
			$g['AR3'] = (double)str_replace(',','',$test[2]);
			$g['ATOTAL'] =  (double)$test[0] + (double)$test[1] + (double)$test[2];
			$v=0;

			return $g;

		}else if(empty($formula)){
			return array('AR1'=>0,'AR2'=>0,'AR3'=>0);
		}else if($test != '([DONOT_DISPLAY])'){
			$signs = array();
			preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9+*-\.\/]+)?\)/',$formula,$matches,PREG_PATTERN_ORDER);
			
			preg_match_all('/\)[+\/*-]\(/',$formula,$matches2,PREG_PATTERN_ORDER);
			//echo '<pre>';print_r($matches2);echo '</pre>';
			$signs=$matches2[0];
			$all_arrays = array();
			$totals = 0;
			$m = array();
			foreach($matches[0] as $ap){
				//removing multiple
				$ab = $ap;			
				//$aa = preg_split('/[+-\/*]/', $ap);
				$ap = "@@@@".$ap."@@@@";

				$ap = str_replace('@@@@([', '', $ap);
				$ap = str_replace('])@@@@', '', $ap);
				$ap = str_replace(']*', '*', $ap);
				$ap = str_replace(']/', '/', $ap);
				$ap = str_replace(']-', '-', $ap);
				$ap = str_replace(']+', '+', $ap);
				$ap = str_replace(')', '', $ap);
				$ap = str_replace('@@@@', '', $ap);
				//echo '<br/>'.$ap = str_replace(')@@@@', '', $ap);
				//echo '<br/>'.$ap = str_replace(']', '', $ap);
				
				//echo '<br/>b ap: '.$ap;

				preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);
				//print_r($matches3);
				$nas = $matches3[0][0];
				$ap = str_replace($nas, '', $ap);
				$p = explode('@',$ap);
				$number_and_sign[$totals] = $nas;
				
				$m[$totals][0] = $ab;//formula
				$m[$totals][1] = $ap;
				$m[$totals][2] = $nas;
				$type = $p[0];
				$po = $p[1];
				$cust = $this->rgf("metering_point", $po, "mp_id", "mp_customer_id");
				//echo "##################".$credit_note."##################";
				$r = $this->get_readings($type, $cust, $po, $month, $year, $credit_note);
				$all_arrays[] = $r;
				$totals++;
			}

			// echo '<pre>';
			// print_r($all_arrays);
			// echo '</pre>';
			$g = array('R1','R2','R3','TOTAL');
			$formulas = array();
			$formulas_eval = array();
			foreach($g as $gg){
				$string = "";
				$string_to_eval = "";
				for($i=0; $i<$totals; $i++){
					//echo "$i $gg";
					if($m[$i][0]=='(['.$m[$i][1].'])'){
						$string_to_eval .= ($all_arrays[$i][$gg]);
					}else{
						$string_to_eval .= str_replace('['.$m[$i][1].']', ($all_arrays[$i][$gg]), $m[$i][0]);
					}
					if($i<$totals-1){
						$s = $signs[$i];
						$s = str_replace(')',"", $s);
						$s = str_replace('(',"", $s);
						$string_to_eval .=$s;
					}
				}

				$string_to_eval = str_replace("-0", "", $string_to_eval);
				$string_to_eval = str_replace("--", "+", $string_to_eval); 
				$formulas_eval[]= $string_to_eval;
			}
			$Cal = new Field_calculate();
			$v = 0;
			foreach($g as $gg){
				$vvv['A'.$gg]=$Cal->Calculate($formulas_eval[$v++]);
			}

		}
		return $vvv;
	}


	public function total_tlf($year, $month, $customer_id){
		
		$count = 1;
		$db = new Db();

		//$tou = $this->tou(2019, $month, $year);
		$tou = $this->tou(2021, $month, $year);

		$peak = $tou[0];
		$shoulder = $tou[1];
		$off_peak = $tou[2];

		$tou = array($shoulder, $peak, $off_peak, 0);	


		$time_readings = strtotime(date($year.'-'.$month.'-15'));
		$sql = "SELECT rate_tlfs, rea_date, rea_id, mp_id,mp_location, region_id FROM r_rate, metering_point, meter_region, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = region_id AND rea_date = '$time_readings' AND rea_id = rate_reading_id AND (rate_tlf IS NOT NULL AND rate_tlf != '') ORDER BY region_order ASC";
		$select = $db->select($sql);

		$v = array();
		$v2 = array();
		$meters = array();
		$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;
		foreach($select[0] as $row){
			extract($row);
				//selecting rates
			$meters[] = strtoupper($mp_location);
				
			$v = $this->get_advance($customer_id, $mp_id, $month, $year, $rate_tlfs);

			if($region_id == 8 && $customer_id == 2019 || $region_id == 10){					
				$g0 -= $v['AR1']*$tou[0]/1000;
				$g1 -= $v['AR2']*$tou[1]/1000;
				$g2 -= $v['AR3']*$tou[2]/1000;
				$g3 = $g0+$g1+$g2;
			}else{				
				$g0 += $v['AR1']*$tou[0]/1000;
				$g1 += $v['AR2']*$tou[1]/1000;
				$g2 += $v['AR3']*$tou[2]/1000;
				$g3 = $g0+$g1+$g2;	
			}
		
		}
		
		$cus = implode(', ', $meters);

		return array('tlf'=>$g3, 'customers'=>$cus);

	}

	
	public function total_payable($year, $month, $customer_id, $credit_note=0){
		$payable = array();

		if($credit_note == 1){


				$customer_payables = array();

				$time_readings = strtotime(date($year.'-'.$month.'-15'));
				$db = new Db();
				
				$sql = "SELECT n_rea_mp_id, n_rate_wc_sfi,n_rate_wc_sfe FROM r_rate_cn,r_reading_cn WHERE n_rea_id = n_rate_reading_id AND n_rea_date = '$time_readings' AND n_rate_cus_id = '$customer_id' AND (n_rate_wc_sfi IS NOT NULL OR n_rate_wc_sfe IS NOT NULL) AND n_rate_lfi != '([DONOT_DISPLAY])' AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )";

				$customer_payables = array();
				$select = $db->select($sql);
				foreach($select[0] as $row){
					extract($row);					
					$wc = $this->get_advanceWheeling($customer_id, $n_rea_mp_id, $month, $year, $n_rate_wc_sfi, 0, 1);
					if($n_rate_wc_sfi){
						$rate = explode('=>',$n_rate_wc_sfi);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$rate][] = $wc;
					}

					$wc = $this->get_advanceWheeling($customer_id, $n_rea_mp_id, $month, $year, $rate_wc_sfe, 0, 1);
					if($n_rate_wc_sfe){
						$rate = explode('=>',$n_rate_wc_sfe);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$rate][] = $wc;
					}

				}

		}else{

			$time_readings = strtotime(date($year.'-'.$month.'-15'));
			$db = new Db();

			$sql = "SELECT rea_mp_id, rate_wc_sfi,rate_wc_sfe FROM r_rate,r_reading WHERE rea_id = rate_reading_id AND rea_date = '$time_readings' AND rate_cus_id = '$customer_id' AND (rate_wc_sfi IS NOT NULL OR rate_wc_sfe IS NOT NULL) AND rate_lfi != '([DONOT_DISPLAY])'";


			$customer_payables = array();
			$select = $db->select($sql);
			foreach($select[0] as $row){
				extract($row);					
				$wc = $this->get_advanceWheeling($customer_id, $rea_mp_id, $month, $year, $rate_wc_sfi);
				if($rate_wc_sfi){
					$rate = explode('=>',$rate_wc_sfi);
					$rate = str_replace('([', '', $rate[1]);
					$rate = str_replace('])', '', $rate);
					$customer_payables[$rate][] = $wc;
				}

				$wc = $this->get_advanceWheeling($customer_id, $rea_mp_id, $month, $year, $rate_wc_sfe);
				if($rate_wc_sfe){
					$rate = explode('=>',$rate_wc_sfe);
					$rate = str_replace('([', '', $rate[1]);
					$rate = str_replace('])', '', $rate);
					$customer_payables[$rate][] = $wc;
				}

			}

		}

		$tot1=$tot2=$tot3=$tot4=$tot5=0;
		$cusArray = array();
		foreach($customer_payables as $cus=>$pay){
			$cusArray[] = $cus;
			$tAR1 = $tAR2 = $tAR3 = $tATOTAL = $tAMR1 = $tAMR2 = $tAMR3 = $tALL = 0;
			foreach($pay as $x){
				extract($x);
				$tAR1 += $AR1;
				$tAR2 += $AR2;
				$tAR3 += $AR3;
				$tATOTAL += $ATOTAL;
				$tAMR1 += $AMR1;
				$tAMR2 += $AMR2;
				$tAMR3 += $AMR3;
				$tALL += $AMR1+$AMR2+$AMR3;
			}

			$tot1 += $tAR1;
			$tot2 += $tAR2;
			$tot3 += $tAR3;
			$tot4 += $tATOTAL;
			$tot5 += $tALL;
		}
		$cus = implode(', ', $cusArray);
		$payable = $tot5;

		return array('payable'=>$tot5, 'customers'=>$cus);
	}
	

	public function total_collectable($year, $month, $customer_id, $credit_note = 0){
		$payable = array();

		if($credit_note == 1){
			$customer_payables = array();

				$time_readings = strtotime(date($year.'-'.$month.'-15'));
				$db = new Db();
				$sql = "SELECT n_rea_cus_id, n_rea_mp_id, n_rate_wc_sfi,n_rate_wc_sfe FROM r_rate_cn,r_reading_cn WHERE n_rea_id = n_rate_reading_id AND n_rea_date = '$time_readings' AND (n_rate_payable_imports = '$customer_id' OR n_rate_payable_exports = '$customer_id') AND (n_rate_wc_sfi IS NOT NULL OR n_rate_wc_sfe IS NOT NULL) AND n_rate_lfi != '([DONOT_DISPLAY])'";
				$customer_payables = array();
				$select = $db->select($sql);
				foreach($select[0] as $row){
					extract($row);					
					$wc = $this->get_advanceWheeling($n_rea_cus_id, $n_rea_mp_id, $month, $year, $n_rate_wc_sfi,$customer_id,1);
					if($n_rate_wc_sfi){
						$rate = explode('=>',$n_rate_wc_sfi);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$this->rgf("customer",$n_rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
					}

					$wc = $this->get_advanceWheeling($n_rea_cus_id, $n_rea_mp_id, $month, $year, $n_rate_wc_sfe,$customer_id,1);
					if($n_rate_wc_sfe){
						$rate = explode('=>',$n_rate_wc_sfe);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$this->rgf("customer",$n_rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
					}

				}

		}else{

			$customer_payables = array();

			$time_readings = strtotime(date($year.'-'.$month.'-15'));
			$db = new Db();
			$sql = "SELECT rea_cus_id, rea_mp_id, rate_wc_sfi,rate_wc_sfe FROM r_rate,r_reading WHERE rea_id = rate_reading_id AND rea_date = '$time_readings' AND (rate_payable_imports = '$customer_id' OR rate_payable_exports = '$customer_id') AND (rate_wc_sfi IS NOT NULL OR rate_wc_sfe IS NOT NULL) AND rate_lfi != '([DONOT_DISPLAY])'";
			$customer_payables = array();
			$select = $db->select($sql);
			foreach($select[0] as $row){
				extract($row);					
				$wc = $this->get_advanceWheeling($rea_cus_id, $rea_mp_id, $month, $year, $rate_wc_sfi,$customer_id);
				if($rate_wc_sfi){
					$rate = explode('=>',$rate_wc_sfi);
					$rate = str_replace('([', '', $rate[1]);
					$rate = str_replace('])', '', $rate);
					$customer_payables[$this->rgf("customer",$rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
				}

				$wc = $this->get_advanceWheeling($rea_cus_id, $rea_mp_id, $month, $year, $rate_wc_sfe,$customer_id);
				if($rate_wc_sfe){
					$rate = explode('=>',$rate_wc_sfe);
					$rate = str_replace('([', '', $rate[1]);
					$rate = str_replace('])', '', $rate);
					$customer_payables[$this->rgf("customer",$rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
				}

			}
		}

		$tot1=$tot2=$tot3=$tot4=$tot5=0;
		$cusArray = array();
		foreach($customer_payables as $cus=>$pay){
			$cusArray[] = $cus;
			$tAR1 = $tAR2 = $tAR3 = $tATOTAL = $tAMR1 = $tAMR2 = $tAMR3 = $tALL = 0;
			foreach($pay as $x){
				extract($x);
				$tAR1 += $AR1;
				$tAR2 += $AR2;
				$tAR3 += $AR3;
				$tATOTAL += $ATOTAL;
				$tAMR1 += $AMR1;
				$tAMR2 += $AMR2;
				$tAMR3 += $AMR3;
				$tALL += $AMR1+$AMR2+$AMR3;
			}

			$tot1 += $tAR1;
			$tot2 += $tAR2;
			$tot3 += $tAR3;
			$tot4 += $tATOTAL;
			$tot5 += $tALL;

		}

		$cus = implode(', ', $cusArray);
		$payable = $tot5;

		return array('collectable'=>$tot5, 'customers'=>$cus);
	}


	public function generateWheelingCharge($customer, $filter_year, $filter_month){
			$customers[] = $customer;

			$time_readings = strtotime(date($filter_year.'-'.$filter_month.'-15'));
			foreach($customers as $customer){
				$db = new Db();
				if(!$this->isLocked($customer, $filter_year, $filter_month)){
					$select = $db->select("SELECT n_rea_id FROM r_reading_cn WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer'");
					
					if($db->num_rows()){
						$errors[] = "Credit Note Readings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
					}
				}else{
					$errors[] = "Readings are locked for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
				}
			}

			if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);
							$db = new Db();
							$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							extract($select[0][0]);

							$insert = $db->insert("r_reading_cn",[
								"n_rea_number"=>$file_name,
								"n_rea_date"=>$time_readings,
								"n_rea_cus_id"=>$customer,
								"n_rea_added_by"=>@$_SESSION['UEDCL_USER_ID'],
								"n_rea_date_added"=>time(),
								"n_rea_mp_id"=>$mp_id,
							]);			



							//GETT READINGS AND FORMULAS ====================

							$db = new Db();
							$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
							extract($select[0][0]);
							$rrr = $rea_id;
							$sql = "SELECT TOP 1 rate_import_wh_1, rate_import_wh_2, rate_import_wh_3, rate_export_wh_4, rate_export_wh_5, rate_export_wh_6, rate_sfi, rate_sfe, rate_lfi, rate_lfe rate_narration, rate_label, rate_wc_sfe, rate_wc_sfi, rate_wc_lfe, rate_wc_lfi, rate_payable_exports, rate_payable_imports, rate_lfe, rate_lfi FROM r_rate, r_reading WHERE rate_cus_id = '$customer' AND rea_id = rate_reading_id AND rea_cus_id = '$customer' AND rea_mp_id = '$mp_id' AND rea_date < '$time_readings' ORDER BY rate_id DESC";
							$select = $db->select($sql);
							extract($select[0][0]);
							//===============================================
									
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 n_rea_id FROM r_reading_cn ORDER BY n_rea_id DESC");
							extract($select[0][0]);
							$rrr = $n_rea_id;
							
							{ //imports as advance
								$select = $db->insert("r_rate_cn",[
									"n_rate_import_wh_1"=>0,
									"n_rate_import_wh_2"=>0,
									"n_rate_import_wh_3"=>0,
									"n_rate_export_wh_4"=>0,
									"n_rate_export_wh_5"=>0,
									"n_rate_export_wh_6"=>0,
									"n_rate_reading_id"=>$rrr,
									"n_rate_added_by"=>$_SESSION['UEDCL_USER_ID'],
									"n_rate_date_added"=>time(),
									"n_rate_cus_id"=>$customer,
									
									"n_rate_sfi"=>$rate_sfi,
									"n_rate_sfe"=>$rate_sfe,
									"n_rate_lfe"=>$rate_lfe,
									"n_rate_lfi"=>$rate_lfi,
									"n_rate_narration"=>$rate_narration,
									"n_rate_label"=>$rate_label,
									"n_rate_wc_sfe"=>$rate_wc_sfe,
									"n_rate_wc_sfi"=>$rate_wc_sfi,
									"n_rate_wc_lfe"=>$rate_wc_lfe,
									"n_rate_wc_lfi"=>$rate_wc_lfi,
									"n_rate_payable_imports"=>$rate_payable_imports,
									"n_rate_payable_exports"=>$rate_payable_exports,
									"n_rate_tlf"=>$rate_tlf,
									"n_rate_tlfs"=>$rate_tlfs,
								]);
							}						

							$insert = $db->insert("r_reading_cn_previous",[
								"n_rea_number"=>$file_name,
								"n_rea_date"=>$time_readings,
								"n_rea_cus_id"=>$customer,
								"n_rea_added_by"=>user_id(),
								"n_rea_date_added"=>time(),
								"n_rea_mp_id"=>$mp_id,
							]);					
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 n_rea_id FROM r_reading_cn_previous ORDER BY n_rea_id DESC");
							extract($select[0][0]);
							$rrr = $n_rea_id;
							{ //imports as advance
								$select = $db->insert("r_rate_cn_previous",[
									"n_rate_import_wh_1"=>$rate_import_wh_1,
									"n_rate_import_wh_2"=>$rate_import_wh_2,
									"n_rate_import_wh_3"=>$rate_import_wh_3,
									"n_rate_export_wh_4"=>$rate_export_wh_4,
									"n_rate_export_wh_5"=>$rate_export_wh_5,
									"n_rate_export_wh_6"=>$rate_export_wh_6,
									"n_rate_reading_id"=>$rrr,
									"n_rate_added_by"=>@$_SESSION['UEDCL_USER_ID'],
									"n_rate_date_added"=>time(),
									"n_rate_cus_id"=>$customer
								]);
							}						

						}
					}

				}

			}
	}

	

	
	public function get_advanceWheeling($customer, $point_id, $month, $year, $formula, $c=0, $credit_note=0){
		$vvv=array();

		//echo $customer.'=>'.$point_id.'=>'.$month.'=>'.$year.'=>'.$formula;

		$formula = explode("=>", $formula);
		$payable_to = $formula[1];
		$formula = $formula[0];

		$test = $formula;

		
		if(strpos($test, "([NetAdvance@(") !== false){

			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			//([NetAdvance@(VALUE_1)(VALUE_2)(VALUE_3)]) 
			$test = str_replace('([NetAdvance@(','',$test);
			$test = str_replace(')])','',$test);
			$test = explode(')(',$test);

			$g = array();
			$g['AR1'] = (double)str_replace(',','',$test[0])/1000.0;
			$g['AR2'] = (double)str_replace(',','',$test[1])/1000.0;
			$g['AR3'] = (double)str_replace(',','',$test[2])/1000.0;


			$g['AMR1'] = (double)str_replace(',','',$test[0])/1000.0*$peak;
			$g['AMR2'] = (double)str_replace(',','',$test[1])/1000.0*$shoulder;
			$g['AMR3'] = (double)str_replace(',','',$test[2])/1000.0*$off_peak;

			$g['AMTOTAL'] =  (double)str_replace(',','',$test[0])/1000.0*$peak + (double)str_replace(',','',$test[1])/1000.0*$shoulder + (double)str_replace(',','',$test[2])/1000.0*$off_peak;

			$g['ATOTAL'] =  (double)str_replace(',','',$test[0])/1000.0 + (double)str_replace(',','',$test[1])/1000.0 + (double)str_replace(',','',$test[2])/1000.0;
			$v=0;

			return $g;

		}else if(empty($formula)){
			return array('AR1'=>0,'AR2'=>0,'AR3'=>0);
		}else if($test != '([DONOT_DISPLAY])'){
			if($customer=='2017' && $c == '2039' && ( $year == '2025' || ($year == '2024' && $month == '12')))
				$tou = $this->tou($c, $month, $year);
			elseif($customer=='2017' && ($c == '2020' || $c == '2040'))
				$tou = $this->tou($c, $month, $year);
			else
				$tou = $this->tou($customer, $month, $year);

			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$tou = array($shoulder, $peak, $off_peak, 0);
			
			$signs = array();
			preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9+*-\.\/]+)?\)/',$formula,$matches,PREG_PATTERN_ORDER);
			
			preg_match_all('/\)[+\/*-]\(/',$formula,$matches2,PREG_PATTERN_ORDER);
			//echo '<pre>';print_r($matches2);echo '</pre>';
			$signs=$matches2[0];
			$all_arrays = array();
			$totals = 0;
			$m = array();
			foreach($matches[0] as $ap){
				//removing multiple
				$ab = $ap;			
				//$aa = preg_split('/[+-\/*]/', $ap);
				$ap = "@@@@".$ap."@@@@";

				$ap = str_replace('@@@@([', '', $ap);
				$ap = str_replace('])@@@@', '', $ap);
				$ap = str_replace(']*', '*', $ap);
				$ap = str_replace(']/', '/', $ap);
				$ap = str_replace(']-', '-', $ap);
				$ap = str_replace(']+', '+', $ap);
				$ap = str_replace(')', '', $ap);
				$ap = str_replace('@@@@', '', $ap);
				//echo '<br/>'.$ap = str_replace(')@@@@', '', $ap);
				//echo '<br/>'.$ap = str_replace(']', '', $ap);
				
				//echo '<br/>b ap: '.$ap;

				preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);
				//print_r($matches3);
				$nas = $matches3[0][0];
				$ap = str_replace($nas, '', $ap);
				$p = explode('@',$ap);
				$number_and_sign[$totals] = $nas;
				
				$m[$totals][0] = $ab;//formula
				$m[$totals][1] = $ap;
				$m[$totals][2] = $nas;
				$type = $p[0];
				$po = $p[1];
				$cust = $this->rgf("metering_point", $po, "mp_id", "mp_customer_id");
				$r = $this->get_readings($type, $cust, $po, $month, $year,$credit_note);
				$all_arrays[] = $r;
				$totals++;
			}
			// echo '<pre>';
			// print_r($all_arrays);
			// echo '</pre>';
			$g = array('R1','R2','R3','TOTAL');
			$formulas = array();
			$formulas_eval = array();
			foreach($g as $gg){
				$string = "";
				$string_to_eval = "";
				for($i=0; $i<$totals; $i++){
					//echo "$i $gg";
					//echo $m[$i][0].'===(['.$m[$i][1].'])';
					if($m[$i][0]=='(['.$m[$i][1].'])'){
					//if(1){
						$string_to_eval .= ($all_arrays[$i][$gg]);
					}else{
						$string_to_eval .= str_replace('['.$m[$i][1].']', ($all_arrays[$i][$gg]), $m[$i][0]);
					}
					if($i<$totals-1){
						$s = $signs[$i];
						$s = str_replace(')',"", $s);
						$s = str_replace('(',"", $s);
						$string_to_eval .=$s;
					}
				}

				$string_to_eval = str_replace("-0", "", $string_to_eval);
				$string_to_eval = str_replace("--", "+", $string_to_eval); 
				$formulas_eval[]= $string_to_eval;
			}
			$Cal = new Field_calculate();
			$v = 0;
			$total_amount = 0;
			foreach($g as $gg){
			
				$amount = $Cal->Calculate($formulas_eval[$v]);//*$tou[$p];
				$vvv['A'.$gg]=$amount/1000;
				$vvv['AM'.$gg]= (double)$amount*(double)$tou[$v]/1000;
				$total_amount += (double)$amount*(double)$tou[$v]/1000;
				
				$v++;
			}
			$payable_to = str_replace("([", "", $payable_to);
			$payable_to = str_replace("])", "", $payable_to);
			$vvv['PAYABLE'] = $payable_to;
			$vvv['AMOUNT'] = $total_amount;
			$vvv['id']= $po;

		}
		return $vvv;
	}


	public function VATCAT($array=0, $index=0){
		$s = array(
			'RES'=>array(
				"Resident (Standard)",
				"18",
				"Standard",
				"A",
			),
			'NRS'=>array(
				"Non Resident (Standard)",
				"18",
				"Standard",
				"A",
			),
			'NRE'=>array(
				"Non Resident (Exempt)",
				"0",
				"Exempt",
				"C",
			),
			'EXP'=>array(
				"Export (Zero Related)",
				"0",
				"Zero Related",
				"B",
			),
			'IMPWHT'=>array(
				"Imported Services + WHT (Standard )",
				"18",
				"Standard - Imported",
				"A",
			),
			'IMP'=>array(
				"Imported Services (Standard)",
				"18",
				"Standard - Imported",
				"A",
			),
		);

		if(empty($array) && empty($id)){
			return $s;
		}else{
			//echo $array;
			return $s[$array][$index];
		}
	}
	public function uom($id){
		if($id == 115) return "Per Month";
		else if($id == 103) return "Kg";
		else if($id == 'KGM') return "KGM-Kilogram";
		else if($id == 'PCE') return "Piece";
		else if($id == '202') return "KWh (Kilo Watt Hour)";
		else return $id;
	}
public function rgf2($table, $cols, $column){
		$db = new Db();

		$look_up = array();
		foreach($cols as $col => $val){
			$look_up[] = $col." = '".str_replace("'", "''", $val)."'";
		}

		$look_up = implode(" AND ", $look_up);

		$sql = "SELECT $column FROM $table WHERE $look_up";
		$row = @$db->select($sql);
		echo $db->error();
		if($db->num_rows())
			return @$row[0][0][$column];
		else
			return '';
	}

	public function start_print($desc){
		echo '<style type="text/css">.t{display: none;}</style>';
		?>

		<span class="pull-right btn btn-primary btn-xs" href="" onclick="printJS({
  printable: 'printMe', 
  type: 'html', 
  style: 'table { width: 100%; }',
  maxWidth: '100%',
  targetStyles: ['box', 'break', 'text-decoration'],
  scanStyles: false,
});"><i class="fa fa-print fa-fw"></i> Print &nbsp; </span>

		<?php 

		echo '<div class="clearfix"></div>';
		echo '<br/>';
		echo '<div id="printMe" class="" style="font-family:arial">';
		echo '<div class="t">';
		echo '<img src="'.return_url().'images/uetcl_header.png" alt="" style="width:100%;margin-bottom:10px;"/>';

		if($desc){
			echo '<div style="width:200px; border:5px double black;margin:auto; padding:2px 5px; text-align: center;">'.$desc.'</div>';
		}

		echo '<div style="font-family:arial; font-size: 10px; text-align: right;line-height: 12px;margin-bottom: 10px;">';
		echo 'Printed By:<b>'.$this->full_name(user_id()).'</b><br/>';
		echo 'Date Printed: <b>'.FeedBack::date_fm(time()).'</b>'; 
		echo '</div>';
		echo '<div class="clearfix"></div>';
		echo '</div>';
		echo '<style>*{font-family:arial;} table tr:hover{background-color:#d8d8d8;}</style>';
	}

	public function end_print(){
		?>
		</div>

		<script>
		function print1(x){
			//if(confirm("Do you really want to print"))
			if(1){					
				var printing = window.open('','','left=0,top=0,width=700,height=400,toolbar=0,scrollbars=0,status=0');
				printing.document.write(document.getElementById('printMe').innerHTML);
				printing.document.close();
				printing.focus();
				printing.print();
				printing.close();
			}else{
				return false;
			}
		}
		</script>
		<?php
	}
	
	public function preview($buyer, $tin, $items, $currency, $qr, $vc, $fdn, $remarks, $operator, $ref, $id){

		$date = $this->rgf("other_invoice", $id, "oi_id", "oi_date");

		if(!$fdn){
		echo '<button type="submit" class="efrisInvoicePreview"><i class="fa fa-fw fa-eye"></i>Show / Hide Preview</button>';

				echo '<div id="efrisInvoiceDetails">';
			}else{
				echo '<div>';
			}
				
				//$this->start_print("TAX INVOICE");

				echo '<div style="width:900px; padding:20px; background-color:white; margin:10px;">';
				echo '<table border="0" cellpadding="5" cellspacing="0" style="width:100%;">';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">e-INVOICE/TAX INVOICE</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section A: Seller\'s Details</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">TIN:</td>';
				// echo '<td colspan="5">1000025097</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2" style="width:300px;">Legal Name:</td>';
				// echo '<td colspan="5">UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Trade Name:</td>';
				// echo '<td colspan="5">UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Address</td>';
				// echo '<td colspan="5">10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I</td>';
				// echo '</tr>';

				echo '<tr>';
				echo '<td style="border:1px solid #000;" colspan="2">Seller\'s Reference Number:</td>';
				echo '<td style="border:1px solid #000;" colspan="5">'.$ref.'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td style="border:1px solid #000;" colspan="2">Served by:</td>';
				echo '<td style="border:1px solid #000;" colspan="5">'.$operator.'</td>';
				echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section B: URA Information</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Document Type:</td>';
				// echo '<td colspan="5">Invoice</td>';
				// echo '</tr>';

				echo '<tr>';
				echo '<td style="border:1px solid #000;" colspan="2">Issued Date:</td>';
				echo '<td style="border:1px solid #000;" colspan="5">'.date('d/M/Y', $date).'</td>';
				echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Time:</td>';
				// echo '<td colspan="5">'.date('h:i:s', $date).'</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Device Number:</td>';
				// echo '<td colspan="5">TCS5ac8fc1155542235</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Fiscal Document Number:</td>';
				// echo '<td colspan="5">'.$fdn.'</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Verification Code:</td>';
				// echo '<td colspan="5">'.$vc.'</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section C: Buyer\'s Details</td>';
				// echo '</tr>';

				echo '<tr>';
				echo '<td style="border:1px solid #000;" colspan="2">Name: </td>';
				echo '<td style="border:1px solid #000;" colspan="5">'.$buyer.'</td>';
				echo '</tr>';

				if($tin){
					echo '<tr>';
					echo '<td style="border:1px solid #000;" colspan="2">TIN: </td>';
					echo '<td style="border:1px solid #000;" colspan="5">'.$tin.'</td>';
					echo '</tr>';
				}

				echo '<tr valign="top" style="border-left:none;">';
				echo '<td colspan="5" style="border-left:none;border-right:none;">';
				echo '<br/><B>URA EFRIS DETAILS:</B><BR/>';
				echo '<table>';
				echo '<tr>';
				echo '<td>Fiscal Document Number:</td>';
				echo '<td>'.$fdn.'</td>';
				echo '</tr>';
				echo '<tr>';
				echo '<td>Verification Code:</td>';
				echo '<td>'.$vc.'</td>';
				echo '</tr>';
				echo '</table><br/>';
				echo '</td>';
				echo '<td colspan="2" style="border-left:none;border-right:none;"><br/><center>'.$qr.'<br/></center></td>';
				echo '</tr>';


				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section D: Goods & Services Details</td>';
				// echo '</tr>';

				echo '<tr>';
				echo '<td style="border:1px solid #000; font-weight:bold;" width="20px;">No. </td>';
				echo '<td style="border:1px solid #000; font-weight:bold;">Item</td>';
				echo '<td style="border:1px solid #000; font-weight:bold;">Quantity</td>';
				echo '<td style="border:1px solid #000; font-weight:bold;">Unit Measure</td>';
				echo '<td style="border:1px solid #000; font-weight:bold;">Unit Price</td>';
				echo '<td style="border:1px solid #000; font-weight:bold;">Total</td>';
				echo '<td style="border:1px solid #000; font-weight:bold;">Tax Category</td>';
				echo '</tr>';
				$number = 0;
				$total = 0;
				foreach($items as $item=>$value){
					$number++;
					echo '<tr>';
					echo '<td style="border:1px solid #000;"width="20px;">'.$value[0].' </td>';
					echo '<td style="border:1px solid #000;">'.$value[1].' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($value[2],2).' </td>';
					echo '<td style="border:1px solid #000; text-align:left;">'.$this->uom($value[5]).' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($value[3],2).' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($value[4],2).' </td>';
					echo '<td style="border:1px solid #000; text-align:left;">'.$this->VATCAT($this->rgf("other_invoice",$id, "oi_id", "oi_vat"), 3).' </td>';
					echo '</tr>';
					$total += $value[4];
				}

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section E: Tax Details</td>';
				// echo '</tr>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td style="font-weight:bold;" colspan="2" width="20px;">Tax Category</td>';
				// echo '<td style="font-weight:bold;">Net Amount</td>';
				// echo '<td style="font-weight:bold;" colspan="2">Tax Amount</td>';
				// echo '<td style="font-weight:bold;" colspan="2">Gross Amount</td>';
				// echo '</tr>';

				// echo '</tr>';
				
				$oi_vat = $this->rgf("other_invoice", $id, "oi_id", "oi_vat");
				$tax_rate =($oi_vat == "RES" || $oi_vat == 'NRS' || $oi_vat == 'IMP' || $oi_vat == 'IMPWHT') ? 18/100 : 0;
				
				$c = (is_infinite($total/$tax_rate))? 0 : $total/$tax_rate;

				// echo '<tr>';
				// echo '<td colspan="2" width="20px;">'.$this->VATCAT($this->rgf("other_invoice",$id, "oi_id", "oi_vat"), 3).': '.$this->VATCAT($this->rgf("other_invoice",$id, "oi_id", "oi_vat"), 2).' ('.$this->VATCAT($this->rgf("other_invoice",$id, "oi_id", "oi_vat"), 1).'%)</td>';
				// echo '<td style="text-align:right;">'.number_format($total/(1+$tax_rate),4).'</td>';
				// echo '<td colspan="2" style="text-align:right;">'.number_format($total/(1+$tax_rate)*$tax_rate,4).'</td>';
				// echo '<td colspan="2" style="text-align:right;">'.number_format($total,4).'</td>';
				// echo '</tr>';

				echo '<tr>';
				echo '<td colspan="7" style="text-align:center;font-weight:bold;"><br/></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Net Amount: </td>';
				echo '<td colspan="5">'.number_format($total/(1+$tax_rate),2).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Tax Amount: </td>';
				echo '<td colspan="5">'.number_format($total/(1+$tax_rate)*$tax_rate,2).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Gross Amount: </td>';
				echo '<td colspan="5">'.number_format($total,2).'<br/>'.FeedBack::withCents($total, $this->rgf("dictionary", $this->rgf("other_invoice",$id, "oi_id", "oi_currency"), "d_code", "d_name")).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Currency: </td>';
				echo '<td colspan="5">'.$this->rgf("dictionary", $this->rgf("other_invoice",$id, "oi_id", "oi_currency"), "d_code", "d_name").'</td>';
				echo '</tr>';

				if($this->rgf("dictionary", $this->rgf("other_invoice",$id, "oi_id", "oi_currency"), "d_code", "d_name")=="USD"){
					echo '<tr>';
					echo '<td colspan="2">Exchange Rate: </td>';
					echo '<td colspan="5">'.number_format($this->rgf("other_invoice",$id, "oi_id", "oi_exchange_rate"),4).'</td>'; 
					echo '</tr>';
				}

				echo '<tr>';
				echo '<td colspan="2">Remarks: </td>';
				echo '<td colspan="5">'.$remarks.'</td>';
				echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="font-weight:bold;text-align:center;">*** END OF e-INVOICE/TAX INVOICE ***</td>';
				// echo '</tr>';


				echo '</table>';
				 //$this->end_print();
				echo '</div>';

				if(!$fdn){
					echo '<button type="submit" class="efrisInvoicePreview"><i class="fa fa-fw fa-eye"></i>Show / Hide Preview</button>';
				}
				echo '</div>';
			?>
			<script type="text/javascript">
				$(document).ready(function(){
					$('#efrisInvoiceDetails').hide();
					$(".efrisInvoicePreview").click(function(){
						$("#efrisInvoiceDetails").toggle();
					});
				});
			</script>
			<?php
	}

	public function sumFormulaeOLD($customer_id, $year, $month, $credit_note=0){
		//echo $customer_id.' '.$year.' '.$month;
		
		$count = 1;
		$db = new Db();

		$time = strtotime($year.'-'.$month.'-15');

		if($credit_note){
			$select = $db->select("SELECT n_rea_date as rea_date, n_rea_id, mp_id, region_id  FROM metering_point, meter_region, r_reading_cn, r_rate_cn WHERE n_rate_reading_id = n_rea_id AND (n_rate_sfe != '([DONOT_DISPLAY])' OR n_rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND n_rea_date = '$time' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC");
		}else{
			$select = $db->select("SELECT rea_date, rea_id, mp_id, region_id FROM metering_point, meter_region, r_reading, r_rate WHERE rate_reading_id = rea_id AND (rate_lfi != '([DONOT_DISPLAY])' AND rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND rea_date = '$time' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC");
		}

		$v = array();
		$v2 = array();
		$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$d = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'";
					$sel = $d->select($sql);
				}else{
					$sql = "SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id'";
					$sel = $d->select($sql);
				}

				if($d->num_rows()){
					@extract($sel[0][0]);

					if($credit_note){
						$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe, 1);
					}else{
						$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe);
						//print_r($v);
					}
				}

				$d = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id' ";
					$sel = $d->select($sql);
				}else{
					$sel = $d->select("SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id'");
				}

				if($d->num_rows()){
					@extract($sel[0][0]);

					if($credit_note){
						$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi, 1);
					}else{
						$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi);
					}
				}

				if($region_id == 8 && $customer_id == 2019 || $region_id == 10){					
					$g0 -= $v['AR1'];
					$g1 -= $v['AR2'];
					$g2 -= $v['AR3'];
					$g3 -= $v['ATOTAL'];
					$g4 -= $v2['AR1'];
					$g5 -= $v2['AR2'];
					$g6 -= $v2['AR3'];
					$g7 -= $v2['ATOTAL'];
				}else{				
					$g0 += $v['AR1'];
					$g1 += $v['AR2'];
					$g2 += $v['AR3'];
					$g3 += $v['ATOTAL'];
					$g4 += $v2['AR1'];
					$g5 += $v2['AR2'];
					$g6 += $v2['AR3'];
					$g7 += $v2['ATOTAL'];	
				}

				//$t = new BeforeAndAfter();
				//if($customer_id == 2036)
				//echo $t->rgf("metering_point",$mp_id, "mp_id","mp_location").'=='.$g0.'=='.$rate_lfi.'=='.$rea_id.'<Br/>';
			}	
		}

		$sum = array($g0, $g1, $g2, $g3, $g4, $g5, $g6, $g7);
		return $sum;

	}

	public function sumFormulae($customer_id, $year, $month, $credit_note=0){
		//echo $customer_id.' '.$year.' '.$month;
		
		$count = 1;
		$db = new Db();

		$time = strtotime($year.'-'.$month.'-15');

		if($credit_note){
			$select = $db->select("SELECT n_rea_date as rea_date, n_rea_id, mp_id, region_id  FROM metering_point, meter_region, r_reading_cn, r_rate_cn WHERE n_rate_reading_id = n_rea_id AND (n_rate_sfe != '([DONOT_DISPLAY])' OR n_rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND n_rea_date = '$time' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = region_id AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 ) ORDER BY region_order ASC");
		}else{
			$sql = "SELECT rea_date, rea_id, mp_id, region_id FROM metering_point, meter_region, r_reading, r_rate WHERE rate_reading_id = rea_id AND (rate_sfe != '([DONOT_DISPLAY])' OR rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND rea_date = '$time' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC";

			$select = $db->select($sql);

			//echo '<br/> >>>>'.$sql.'... ='.	$db->num_rows();
		}

		$v = array();
		$v2 = array();
		$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$d = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'";
					$sel = $d->select($sql);
				}else{
					$sql = "SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id'";
					$sel = $d->select($sql);

					//echo ' >>>>'.$d->num_rows();
				}

				if($d->num_rows()){
					@extract($sel[0][0]);

					if($credit_note){
						$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe, 1);
					}else{
						$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe);
						//print_r($v);
					}
				}

				$d = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id' ";
					$sel = $d->select($sql);
				}else{
					$sel = $d->select("SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id'");
				}

				if($d->num_rows()){
					@extract($sel[0][0]);

					if($credit_note){
						$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi, 1);
					}else{
						$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi);
					}
				}

				if($region_id == 8 && ($customer_id == 2019 || $customer_id == 2040) || $region_id == 10){					
					$g0 -= $v['AR1'];
					$g1 -= $v['AR2'];
					$g2 -= $v['AR3'];
					$g3 -= $v['ATOTAL'];
					$g4 -= $v2['AR1'];
					$g5 -= $v2['AR2'];
					$g6 -= $v2['AR3'];
					$g7 -= $v2['ATOTAL'];
				}else{				
					$g0 += $v['AR1'];
					$g1 += $v['AR2'];
					$g2 += $v['AR3'];
					$g3 += $v['ATOTAL'];
					$g4 += $v2['AR1'];
					$g5 += $v2['AR2'];
					$g6 += $v2['AR3'];
					$g7 += $v2['ATOTAL'];	
				}
			}	
		}

		$sum = array($g0, $g1, $g2, $g3, $g4, $g5, $g6, $g7);
		return $sum;

	}
	


	public function sumFormulaeBack($customer_id, $year, $month, $credit_note=0){
		//echo $customer_id.' '.$year.' '.$month;
		
		$count = 1;
		$db = new Db();

		$time = strtotime($year.'-'.$month.'-15');

		if($credit_note){
			$select = $db->select("SELECT n_rea_date as rea_date, n_rea_id, mp_id, region_id  FROM metering_point, meter_region, r_reading_cn, r_rate_cn WHERE n_rate_reading_id = n_rea_id AND (n_rate_sfe != '([DONOT_DISPLAY])' OR n_rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND n_rea_date = '$time' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC");
		}else{
			$select = $db->select("SELECT rea_date, rea_id, mp_id, region_id FROM metering_point, meter_region, r_reading, r_rate WHERE rate_reading_id = rea_id AND (rate_lfi != '([DONOT_DISPLAY])' AND rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND rea_date = '$time' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC");
		}

		$v = array();
		$v2 = array();
		$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$d = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'";
					$sel = $d->select($sql);
				}else{
					$sql = "SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id'";
					$sel = $d->select($sql);
				}

				if($d->num_rows()){
					@extract($sel[0][0]);

					if($credit_note){
						$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe, 1);
					}else{
						$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe);
						//print_r($v);
					}
				}

				$d = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id' ";
					$sel = $d->select($sql);
				}else{
					$sel = $d->select("SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id'");
				}

				if($d->num_rows()){
					@extract($sel[0][0]);

					if($credit_note){
						$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi, 1);
					}else{
						$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi);
					}
				}

				if($region_id == 8 && $customer_id == 2019 || $region_id == 10){					
					$g0 -= $v['AR1'];
					$g1 -= $v['AR2'];
					$g2 -= $v['AR3'];
					$g3 -= $v['ATOTAL'];
					$g4 -= $v2['AR1'];
					$g5 -= $v2['AR2'];
					$g6 -= $v2['AR3'];
					$g7 -= $v2['ATOTAL'];
				}else{				
					$g0 += $v['AR1'];
					$g1 += $v['AR2'];
					$g2 += $v['AR3'];
					$g3 += $v['ATOTAL'];
					$g4 += $v2['AR1'];
					$g5 += $v2['AR2'];
					$g6 += $v2['AR3'];
					$g7 += $v2['ATOTAL'];	
				}

				//$t = new BeforeAndAfter();
				//if($customer_id == 2036)
				//echo $t->rgf("metering_point",$mp_id, "mp_id","mp_location").'=='.$g0.'=='.$rate_lfi.'=='.$rea_id.'<Br/>';
			}	
		}

		$sum = array($g0, $g1, $g2, $g3, $g4, $g5, $g6, $g7);
		return $sum;

	}
	


	public function sumFormulaeBackflow($customer_id, $year, $month, $credit_note=0){
		
		$count = 1;
		$db = new Db();

		$time = strtotime($year.'-'.$month.'-15');

		if($credit_note){
			$select = $db->select("SELECT n_rea_date as rea_date, n_rea_id, mp_id, region_id  FROM metering_point, meter_region, r_reading_cn, r_rate_cn WHERE n_rate_reading_id = n_rea_id AND ( n_rate_sfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND n_rea_date = '$time' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC");
		}else{
			$select = $db->select("SELECT rea_date, rea_id, mp_id, region_id FROM metering_point, meter_region, r_reading, r_rate WHERE rate_reading_id = rea_id AND (rate_lfi != '([DONOT_DISPLAY])' AND rate_lfe != '([DONOT_DISPLAY])' ) AND mp_customer_id = '$customer_id' AND rea_date = '$time' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = region_id ORDER BY region_order ASC");
		}

		$v = array();
		$v2 = array();
		$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;

		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$db = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id' AND ( n_rate_lfi != '([DONOT_DISPLAY])' AND rate_lfe != '([DONOT_DISPLAY])' )";
					$sel = $db->select($sql);
				}else{
					$sel = $db->select("SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id' AND ( rate_lfi != '([DONOT_DISPLAY])' AND rate_lfe != '([DONOT_DISPLAY])' )");
				}

				@extract($sel[0][0]);

				if($credit_note){
					$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe, 1);
					$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi, 1);
				}else{
					$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe);
					$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi);
				}

				if($region_id == 8 && $customer_id == 2019 || $region_id == 10 && $customer_id != 2036){					
					$g0 -= $v['AR1'];
					$g1 -= $v['AR2'];
					$g2 -= $v['AR3'];
					$g3 -= $v['ATOTAL'];
					$g4 -= $v2['AR1'];
					$g5 -= $v2['AR2'];
					$g6 -= $v2['AR3'];
					$g7 -= $v2['ATOTAL'];
				}else{				
					$g0 += $v['AR1'];
					$g1 += $v['AR2'];
					$g2 += $v['AR3'];
					$g3 += $v['ATOTAL'];
					$g4 += $v2['AR1'];
					$g5 += $v2['AR2'];
					$g6 += $v2['AR3'];
					$g7 += $v2['ATOTAL'];	
				}


			}	
		}

		$sum = array($g0, $g1, $g2, $g3, $g4, $g5, $g6, $g7);
		return $sum;

	}
	


	public function sumFormulaeX($customer_id, $year, $month, $credit_note=0){
		
		$count = 1;
		$db = new Db();

		$time = strtotime($year.'-'.$month.'-15');
		$meters = array();

		$sql = "SELECT n_rea_mp_id,n_rate_sfe from r_reading_cn, r_rate_cn WHERE n_rea_date = '$time' AND n_rea_id = n_rate_reading_id AND n_rea_cus_id = '$customer_id' AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )";
		$select = $db->select($sql);
		if($db->num_rows()){
			foreach($select[0] as $row){
				extract($row);
				$v = $this->get_advance($customer_id, $n_rea_mp_id, $month, $year, $n_rate_sfe, 1);

				$g0 = ($v['AR1']);
				$g1 = ($v['AR2']);
				$g2 = ($v['AR3']);

				$g3 = $g1 + $g2 + $g0;

				if($g3 != 0){
					$meters[] = "mp_id = '$n_rea_mp_id'";
				}
			}
		}

		$allmeters = "";
		if(count($meters)){
			$allmeters = ' AND ( '.implode(' OR ', $meters).' )';
		}else{
			$allmeters = ' AND 0 = 1';
		}


		$db = new Db();
		if($credit_note){
			$select = $db->select("SELECT n_rea_date as rea_date, n_rea_id, mp_id, region_id  FROM metering_point, meter_region, r_reading_cn, r_rate_cn WHERE n_rate_reading_id = n_rea_id AND (n_rate_sfi != '([DONOT_DISPLAY])' OR n_rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND n_rea_date = '$time' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = region_id $allmeters ORDER BY region_order ASC");
		}else{
			$sql = "SELECT rea_date, rea_id, mp_id, region_id FROM metering_point, meter_region, r_reading, r_rate WHERE rate_reading_id = rea_id AND (rate_sfi != '([DONOT_DISPLAY])' OR rate_lfe != '([DONOT_DISPLAY])') AND mp_customer_id = '$customer_id' AND rea_date = '$time' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = region_id $allmeters ORDER BY region_order ASC";
			$select = $db->select($sql);
		}

		$v = array();
		$v2 = array();
		$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;
		foreach($select[0] as $row){
			extract($row);
			//echo $mp_id.'== ';
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$db = new Db();

				if($credit_note){
					$sql = "SELECT n_rate_sfi as sfi,n_rate_sfe as sfe FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id' AND (n_rate_sfi != '([DONOT_DISPLAY])' OR n_rate_lfe != '([DONOT_DISPLAY])')";
					$sel = $db->select($sql);
				}else{
					$sel = $db->select("SELECT rate_sfi as sfi,rate_sfe as sfe FROM r_rate WHERE rate_reading_id = '$rea_id' AND (rate_sfi != '([DONOT_DISPLAY])' OR rate_lfe != '([DONOT_DISPLAY])')");
				}

				@extract($sel[0][0]);

				if($credit_note){
					$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe, 1);
					$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi, 1);
				}else{
					$v = $this->get_advance($customer_id, $mp_id, $month, $year, $sfe);
					$v2 = $this->get_advance($customer_id, $mp_id, $month, $year, $sfi);
				}

				if($region_id == 8 && $customer_id == 2019 || $region_id == 10){					
					$g0 -= $v['AR1'];
					$g1 -= $v['AR2'];
					$g2 -= $v['AR3'];
					$g3 -= $v['ATOTAL'];
					$g4 -= $v2['AR1'];
					$g5 -= $v2['AR2'];
					$g6 -= $v2['AR3'];
					$g7 -= $v2['ATOTAL'];
				}else{				
					$g0 += $v['AR1'];
					$g1 += $v['AR2'];
					$g2 += $v['AR3'];
					$g3 += $v['ATOTAL'];
					$g4 += $v2['AR1'];
					$g5 += $v2['AR2'];
					$g6 += $v2['AR3'];
					$g7 += $v2['ATOTAL'];	
				}
			}	
		}

		$sum = array($g0, $g1, $g2, $g3, $g4, $g5, $g6, $g7);
		return $sum;

	}
	
	
		
	
	
	public function get_readings21($customer, $point_id, $month, $year, $formula){

		$test = $formula;
		if($rate_lfi != '([DONOT_DISPLAY])'){
			$signs = array();
			preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9+*-]+)?\)/',$formula,$matches,PREG_PATTERN_ORDER);
			
			preg_match_all('/\)[+\/*-]\(/',$formula,$matches2,PREG_PATTERN_ORDER);
			//echo '<pre>';print_r($matches2);echo '</pre>';
			$signs=$matches2[0];
			$all_arrays = array();
			$totals = 0;
			$m = array();
			foreach($matches[0] as $ap){
				//removing multiple
				$ab = $ap;			
				$aa = preg_split('/[+-\/*]/', $ap);
				$ap = $aa[0]."@@@@";
				$ap = str_replace('([', '', $ap);
				$ap = str_replace(')]', '', $ap);
				$ap = str_replace(')*', '*', $ap);
				$ap = str_replace(')/', '/', $ap);
				$ap = str_replace(')@@@@', '', $ap);
				$ap = str_replace(']', '', $ap);
				
				$p = explode('@',$ap);
				
				$m[$totals][0] = $ab;//formula
				$m[$totals][1] = $ap;
				$type = $p[0];
				$po = $p[1];
				$cust = $this->rgf("metering_point", $po, "mp_id", "mp_customer_id");
				$r = $this->get_readings($type, $cust, $po, $month, $year);
				$all_arrays[] = $r;
				$totals++;
			}
			// echo '<pre>';
			// print_r($all_arrays);
			// echo '</pre>';
			$g = array('R1','R2','R3','TOTAL');
			$formulas = array();
			$formulas_eval = array();
			foreach($g as $gg){
				$string = "";
				$string_to_eval = "";
				for($i=0; $i<$totals; $i++){
					//echo "$i $gg";
					if($m[$i][0]=='(['.$m[$i][1].'])'){
						$string .= number_format($all_arrays[$i][$gg]/1000);
						$string_to_eval .= ($all_arrays[$i][$gg]/1000);
					}else{
						$string .= str_replace('['.$m[$i][1].']', number_format($all_arrays[$i][$gg]/1000), $m[$i][0]);
						$string_to_eval .= str_replace('['.$m[$i][1].']', ($all_arrays[$i][$gg]/1000), $m[$i][0]);
					}
					if($i<$totals-1){
						$s = $signs[$i];
						$s = str_replace(')',"", $s);
						$s = str_replace('(',"", $s);
						$string .= " ".$s."&nbsp;";
						$string_to_eval .=$s;
					}
				}

				$string_to_eval = str_replace("-0", "", $string_to_eval);
				$string_to_eval = str_replace("--", "+", $string_to_eval); 

				$formulas[]= $string;
				$formulas_eval[]= $string_to_eval;
			}
			$Cal = new Field_calculate();
			$v = 0;
			$t=array();
			foreach($g as $gg){
				$t[] = '<b>A'.$gg.'</b> = <b>'.number_format($Cal->Calculate($formulas_eval[$v])).'</b>'.($formulas[$v++]).'';
			}
		}else{
			$t=array("");
		}

		return $t;
	}
	
	public function decimalFixer($number){
		$num = explode('.', $number);

		if(empty($num[1])){
			return number_format($num[0]);
		}else{			
			return number_format($num[0]).'.'.$num[1];
		}
	}

	public function energyLinks($id, $year, $month){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);
		$note = ($this->set_note)?$this->set_note: portion(6);


		echo '<div class="nav files pull-left">';
		$t=0;

		if($id == 2030)
			$wcs = 'Bill';
		else 
			$wcs = 'Advance Schedule';


		if(!empty($note)){

			if($this->isDNorCN($id, $year, $month) > 0){
				$word = "CN/DN ";
				$word_full = "Dedit Note";
				$t=1;
			}else{
				$word = "CN/DN ";
				$word_full = "Credit Note";
				$t=1;
			}


			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			if($method == "energy-customer-details"){

			}

			if($method != "previous-readings"){ 
				echo '<a class="eagle-load" accesskey="e" href="'.return_url().'services/previous-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-edit fa-fw"></i>'.$word.' Previous Readings</a>';
			} else { 
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-edit fa-fw"></i>'.$word.' Previous Readings</span>';
			}
			
			echo '&nbsp; &nbsp;'; 

			if($method != "readings"){ 
				echo '<a class="eagle-load" accesskey="e" href="'.return_url().'services/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-edit fa-fw"></i>'.$word.' Current Readings</a>';
			} else { 
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-edit fa-fw"></i>'.$word.' Current Readings</span>';
			}
			
			echo '&nbsp; &nbsp;'; 
			if($method != "formulae"){ 
				echo '<a class="eagle-load" accesskey="b" href="'.return_url().'formulae-builder/formulae/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-calculator fa-fw"></i>'.$word.' Formulae Builder</a>';
			} else { 
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-calculator fa-fw"></i>'.$word.' Formulae Builder</span>';
			}
			echo '&nbsp; &nbsp;'; 


			if(0){//BILL AND AMOUNT FOR TANESCO
				echo ' &nbsp; &nbsp;'; 
				if($method != "generator-bill-and-amount"){ 
					echo '<a class="eagle-load" title="'.$word_full.' Generator Bill and Amount" style="color:black;" accesskey="b" href="'.return_url().'services/generator-bill-and-amount/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</a>';
				}else{	
					echo '<span title="'.$word_full.' Generator Bill and Amount" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</span>';
				}
			}

			echo ' &nbsp; &nbsp;'; 

			if($method != "schedule"){ 		
				echo '<a class="eagle-load" accesskey="s" href="'.return_url().'services/schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>'.$word.' Schedule</a>'; 	
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i>'.$word.' Schedule</span>'; 
			}

			echo ' &nbsp; &nbsp;'; 

			if($id == 2029 || $id == 2028){				
				echo '';
			}else if($method != "advance-schedule"){ 
				echo '<a class="eagle-load" accesskey="w" href="'.return_url().'services/advance-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>'.$word.' '.$wcs.'</a>';
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' '.$wcs.'</span>';
			}

			if($id == 2019000000){ //replace by backflow customers
				echo ' &nbsp; &nbsp;'; 
				if($method != "backflow-schedule"){ 
					echo '<a class="eagle-load" accesskey="b" href="'.return_url().'services/backflow-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>'.$word.' Backflows</a>';
				}else{	
					echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i>'.$word.' Backflows</span>';
				}
			}

			echo ' &nbsp; &nbsp; ';

			if($method != "view-invoice"){ 
				echo '<a class="eagle-load" accesskey="i" href="'.return_url().'services/view-invoice/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-file fa-fw"></i>'.$word.' Invoice</a>';
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-file fa-fw"></i>'.$word.' Invoice</span>';
			}

			echo '  &nbsp; &nbsp;'; 

			if($method == "energy-customer-details" && $t){

			}else{
				echo '<a class="eagle-load" href="'.return_url().'customers/energy-customer-details/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> Customer Details</a> &nbsp; &nbsp;'; 
			}

			//echo '  &nbsp; &nbsp;'; 

			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		}else{
			

			if($method == "energy-customer-details"){

			}elseif($method != "readings"){ 
				echo '<a class="eagle-load" accesskey="e" href="'.return_url().'services/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-edit fa-fw"></i> Readings</a>';
			} else { 
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-edit fa-fw"></i> Readings</span>';
			}
			
			echo '&nbsp; &nbsp;'; 
			if($method != "formulae"){ 
				echo '<a class="eagle-load" accesskey="b" href="'.return_url().'formulae-builder/formulae/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-calculator fa-fw"></i> Formulae Builder</a>';
			} else { 
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-calculator fa-fw"></i> Formulae Builder</span>';
			}
			echo '&nbsp; &nbsp;'; 


			if(0){//BILL AND AMOUNT FOR TANESCO
				echo ' &nbsp; &nbsp;'; 
				if($method != "generator-bill-and-amount"){ 
					echo '<a class="eagle-load" title="'.$word_full.' Generator Bill and Amount" style="color:black;" accesskey="b" href="'.return_url().'services/generator-bill-and-amount/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</a>';
				}else{	
					echo '<span title="'.$word_full.' Generator Bill and Amount" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</span>';
				}
			}

			echo ' &nbsp; &nbsp;'; 

			if($method != "schedule"){ 		
				echo '<a class="eagle-load" accesskey="s" href="'.return_url().'services/schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>Schedule</a>'; 	
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i>Schedule</span>'; 
			}

			echo ' &nbsp; &nbsp;'; 


			if($id == 2029 || $id == 2028){				
				echo '';
			}else if($method != "advance-schedule"){ 
				echo '<a class="eagle-load" accesskey="w" href="'.return_url().'services/advance-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$wcs.'</a>';
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$wcs.'</span>';
			}

			if($id == 2019000000){ //replace by backflow customers
				echo ' &nbsp; &nbsp;'; 
				if($method != "backflow-schedule"){ 
					echo '<a class="eagle-load" accesskey="b" href="'.return_url().'services/backflow-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>Backflows</a>';
				}else{	
					echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i>Backflows</span>';
				}
			}

			echo ' &nbsp; &nbsp; ';

			if($method != "view-invoice"){ 
				echo '<a class="eagle-load" accesskey="i" href="'.return_url().'services/view-invoice/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-file fa-fw"></i>Invoice</a>';
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-file fa-fw"></i> Invoice</span>';
			}

			echo '  &nbsp; &nbsp;'; 

			if($method == "energy-customer-details" && $t){

			}else{
				echo '<a class="eagle-load" href="'.return_url().'customers/energy-customer-details/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> Customer Details</a> &nbsp; &nbsp;'; 
			}

			//echo '  &nbsp; &nbsp;'; 


		$abc2 = "services";
		/*if(portion(2) == 'loss'){
			echo '&nbsp;<span class="btn-danger btn-xs">Energy Variance </span>&nbsp;';
		}else{
			//'.month_name_short($month).' '.$year.'
			echo '&nbsp;<a href="'.return_url().$abc2.'/loss/'.$id.'/'.$year.'/'.$month.'/" class="btn-xs" style="color:black; background-color: #ffbfff;">Energy Variance </a>&nbsp;';
		}*/

		$abc2 = "formulae-builder";
		if($method == 'fx'){
			//'.month_name_short($month).' '.$year.'
			echo '&nbsp;<span class="btn-danger btn-xs"><i class="fa fa-fx fa-calculator"></i> Fx </span>&nbsp;';
		}else{
			//'.month_name_short($month).' '.$year.'
			echo '&nbsp;<a class="eagle-load" href="'.return_url().$abc2.'/fx/'.$id.'/'.$year.'/'.$month.'/" class="btn-xs" style="color:black; background-color: #ffbfff;"><i class="fa fa-fx fa-calculator"></i> Fx</a>&nbsp;';
		}	

		$abc2 = "formulae-builder";
		if($method == 'fx-details'){
			//Details '.month_name_short($month).' '.$year.'
			echo '&nbsp;<span class="btn-danger btn-xs"><i class="fa fa-fx fa-calculator"></i> Fx Details</span>&nbsp;';
		}else{
			//'.month_name_short($month).' '.$year.'
			echo '&nbsp;<a class="eagle-load" href="'.return_url().$abc2.'/fx-details/'.$id.'/'.$year.'/'.$month.'/" class="btn-xs" style="color:black; background-color: #ffbfff;"><i class="fa fa-fx fa-calculator"></i> Fx Details</a>&nbsp;';
		}

			if($method != "energy-customer-details" 
				&& $method != "readings"  
				&& $method != "debit-note"
				//&& portion(2) != "credit-note"  
				&& $method != "delete-month-readings" 
				&& $method != "new-month-readings" 
				&& $method != "sales-trend"){ //sales-trend
				

				$d = new Db();
				$sql = "SELECT * FROM done_levels WHERE dl_app1 = 1 AND dl_app2 = 1 AND dl_app3 = 1 AND dl_customer = '$id' AND dl_year = '$year' AND dl_month = '$month' ";
				$sel = $d->select($sql);
				if($d->num_rows()||user_id()==10 || 1){
				//if(1){
					echo '<span class="" style="background-color:#000; color:#fff; cursor:pointer; padding:2px 5px; border-radius:5px;" onclick="printJS({
  printable: \'printMe\', 
  type: \'html\', 
  style: \'table { width: 100%; }\',
  maxWidth: \'100%\',
  targetStyles: [\'box\', \'break\', \'text-decoration\'],
  scanStyles: false,
});"><i class="fa fa-print fa-fw"></i> Print </span>';
				}
			}else{
				//echo '<br/><br/>';
			}
		}

		// if(portion(2) == "readings"){
		// 	//echo '<div style="margin-top:10px;">';
		// 	echo '<a href="'.return_url().'services/new-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class=""><i class="fa fa-fx fa-plus"></i> New</a> &nbsp;  &nbsp; ';
		// 	//New Month Readings

		// 	echo '<a href="'.return_url().'services/delete-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="" ><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp; &nbsp; ';
		// 	//Delete All Month Readings

		// 	echo '<a href="'.return_url().'services/debit-note/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="" ><i class="fa fa-fx fa-list"></i> Debit Note</a> &nbsp; &nbsp; ';
		// 	//Delete All Month Readings

		// 	echo '<a href="'.return_url().'services/credit-note/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary" > <i class="fa fa-fx fa-list"></i> Credit Note </a> &nbsp;  &nbsp; ';
		// 	//Delete All Month Readings
		// }

		echo '</div>';
	}

	public function customerAndSchedule($id, $year, $month){
		$abc =  ($this->set_class)?$this->set_class:portion(1);
		$method =  ($this->set_method)?$this->set_method:portion(2);
		$set_customer =  ($this->set_customer)?$this->set_customer:portion(3);
		if($method =="energy-customer-details"){
			$p20 = $p21 = $p22 = "schedule";
		}else{
			$p20 = $p21 = $p22 = $method;
		}
		$customer_id = $id;
		$note = ($this->set_note)?$this->set_note:portion(6);

		echo '<div class="clearfix"></div>';
		$db = new Db();
		echo '<span class="" style="margin:2px 0; color:blue; font-weight: bold;">Customers > </span> &nbsp; ';	
		$select = $db->select("SELECT customer_id as c_id, customer_short_name as c_name FROM customer WHERE customer_status = 1 ORDER BY customer_short_name ASC");
		foreach($select[0] as $row){

			extract($row);
			if($method=="energy-customer-details"){
				if($set_customer == $c_id){
					if($c_id == $id){
						echo '<span class="btn btn-xs btn-danger">'.$c_name.'</span>&nbsp;';
					}else{
						echo '<a href="'.return_url().$abc.'/'.$p20.'/'.$c_id.'/'.$year.'/'.$month.'/'.$note.'" class="btn-xs btn-primary eagle-load">'.$c_name.'</a>&nbsp;';
					}
				}
			}else{
				if($c_id == $id){
					echo '<span class="btn-xs btn-danger">'.$c_name.'</span>&nbsp;';
				}else{
					echo '<a href="'.return_url().$abc.'/'.$p20.'/'.$c_id.'/'.$year.'/'.$month.'/'.$note.'" class="btn-xs btn-primary eagle-load">'.$c_name.'</a>&nbsp;';
				}
			}
			
		}

		echo '<div class="clearfix"></div>';		
		echo '<div style="margin:5px;"></div>';
		echo '<div class="pull-right1">';
		$db = new Db();
		$select = $db->select("SELECT * FROM r_reading ORDER BY rea_date ASC");
				
		$rea_years = array();	

		echo '<span class="" style="color:red; font-weight: bold;">'.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Year> </span> &nbsp; ';		
		foreach($select[0] as $row){
			extract($row);

			if(!in_array(date('Y', $rea_date), $rea_years)){
				$rea_years[] = date('Y', $rea_date);
				if($year == date('Y', $rea_date)){
					echo '<span class="btn-danger btn-xs">'.date('Y', $rea_date).'</span>&nbsp;';
				}else{
					echo '<a href="'.return_url().$abc.'/'.$p21.'/'.$id.'/'.date('Y', $rea_date).'/'.$month.'/'.$note.'" class="btn-xs eagle-load" style="color:black; background-color: #ffbfbf;">'.date('Y', $rea_date).'</a>&nbsp;';
				}
			}
		}

		if($method == 'sales-trend'){
			echo ' &nbsp;<span class="btn-danger btn-xs">Sales Trend '.$year.'</span>&nbsp;';
		}else{
			echo '  &nbsp;<a href="'.return_url().$abc.'/sales-trend/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="btn-xs eagle-load" style="color:black; background-color: #ffbfff;">Sales Trend '.$year.'</a>&nbsp;';
		}		

		echo '</div>';		
		echo '<div style="margin:5px;"></div>';
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY rea_date ASC");
				
		$rea_dates = array();	
		echo '<div class="pull-left1">';
		echo '<span class="" style="color:red; font-weight: bold;">'.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Month> </span> &nbsp; ';	

		//echo '<a href="'.return_url().$abc.'/'.$p22.'/'.$id.'/'.($year-1).'/12/" class="btn-xs" style="color:black; background-color: #ccc;">'.month_name_short(12).' '.($year-1).'</a>&nbsp;';

		foreach($select[0] as $row){
			extract($row);

			if(!in_array($rea_date, $rea_dates) && $year == date('Y', $rea_date)){
				$rea_dates[] = $rea_date;

				$abc12 = "services";
				$invoice_no = str_pad($rea_id, 5, '0', STR_PAD_LEFT);
				if($year == date('Y', $rea_date) && $month == date('m', $rea_date)  && empty($note)){
					echo '<span class="btn-danger btn-xs btn-menu">';
					echo date('M Y', $rea_date);

					echo '<div class=" nav files month-inner-menu">';
					echo '<ul class="">';

					echo '<li>';
					echo '<a href="'.return_url().$abc12.'/new-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="eagle-load"> New Readings</a>';
					echo '</li>';

					echo '<li>';

					if(!$this->isLocked($id, $year, $month)){
						$e = "View / Edit ";
					}else{
						$e = "View ";
					}				
					echo '<a  class="eagle-load" href="'.return_url().$abc12.'/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="eagle-load"> '.$e.' Readings</a>';
					echo '</li>';
					
					if(!$this->isLocked($id, $year, $month)){
						echo '<li>';
						echo '<a href="'.return_url().$abc12.'/delete-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="eagle-load" > Delete Readings</a>';
						echo '</li>';
					}
					
					echo '<li>';
					echo '<a href="'.return_url().$abc12.'/view-invoice/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary eagle-load" >  Invoice </a>';
					echo '</li>';
					
					echo '<li>';

					if(!$this->isLocked($id, $year, $month)){
						echo '<a href="'.return_url().$abc12.'/credit-note/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary eagle-load" >  Debit / Credit Note </a>';
						echo '</li>';
					}

					echo '</ul>';
					echo '</div>';

					echo '</span>&nbsp;';
				}else{
					echo '<a href="'.return_url().$abc.'/'.$p22.'/'.$id.'/'.date('Y', $rea_date).'/'.date('m', $rea_date).'/" class="btn-xs eagle-load" style="color:black; background-color: #ffbfbf;">'.date('M Y', $rea_date).'</a>&nbsp;';
				}
				//echo "SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND n_rea_date = '$rea_date'".'>>>>';
				$t = new Db();
				$tt = $t->select("SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id");
				$n_rea_dates = array();
				foreach($tt[0] as $ttt){
					extract($ttt);

					if(!in_array($n_rea_date, $n_rea_dates) && $year == date('Y', $n_rea_date) && date('m', $rea_date) == date('m', $n_rea_date) )
					{
						$n_rea_dates[] = $n_rea_date;
						if($year == date('Y', $n_rea_date) && $month == date('m', $n_rea_date) && $note =="credit-note"){
							$iDC = ($this->isDNorCN($id, $year, $month) > 0)? "CN/DN":"CN/DN";
							echo '<span class="btn-danger btn-xs btn-menu">';

							echo $iDC.' '.date('M Y', $n_rea_date);

							echo '<div class="month-inner-menu">';
							echo '<ul>';

							echo '<li>';
							echo '<a href="'.return_url().$abc.'/view-invoice/'.$id.'/'.date('Y', $n_rea_date).'/'.date('m', $n_rea_date).'/credit-note" class="primary eagle-load" > View '.$iDC.' Note Invoice </a>';
							echo '</li>';							
							
							echo '<li>';

							if(!$this->isLocked($id, $year, $month)){
								$e = "View / Edit ";
							}else{
								$e = "View ";
							}
							echo '<a href="'.return_url().$abc.'/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary eagle-load" > '.$e.' '.$iDC.' Note Readings </a>';
							echo '</li>';

							echo '<li>';

							if(!$this->isLocked($id, $year, $month)){
								echo '<a href="'.return_url().$abc.'/delete-credit-note-month-readings/'.$id.'/'.date('Y', $n_rea_date).'/'.date('m', $n_rea_date).'/credit-note" class="primary eagle-load" > Delete '.$iDC.' Note </a>';
								echo '</li>';
							}
							
							echo '</ul>';
							echo '</div>';

							echo '</span>&nbsp;';
						}else{
							$iDC = ($this->isDNorCN($id, date('Y', $n_rea_date), date('m', $n_rea_date)) > 0)? "CN/DN":"CN/DN";
							echo '<a href="'.return_url().$abc.'/'.$p22.'/'.$id.'/'.date('Y', $n_rea_date).'/'.date('m', $n_rea_date).'/credit-note" class="btn-xs eagle-load" style="color:black; background-color: #ffbfbf;">'.$iDC.' '.date('M Y', $n_rea_date);


							echo '</a>&nbsp;';
						}
					}
				}
			}
		}	

		//echo '<a href="'.return_url().$abc.'/'.$p22.'/'.$id.'/'.($year+1).'/1/" class="btn-xs" style="color:black; background-color: #ccc;">'.month_name_short(1).' '.($year+1).'</a>&nbsp;';
	

		echo '</div>';

		echo '<div class="clearfix"></div>';

		echo '<div style="margin:5px;"></div>';

		if($method != "schedule" && $note != "credit-note"){

		}elseif($method != "energy-customer-details" 
			&& $method != "readings"  
			&& $method != "debit-note"  
			&& $method != "delete-month-readings" 
			&& $method != "new-month-readings" 
			&& $method != "sales-trend"){ 

			echo '<div class="pull-right" style=" margin-bottom: 20px;">';
			$this->rgf("customer", $id, "customer_id", "customer_default_units");
			echo '<form action="" method="post">';
			if(isset($_POST['change'])){
				$units = $_POST['rate'];
			}else{
				$units = 0.000001;
			}
			echo 'Change Units: ';
			echo '<select name="rate">';

			if($units == "1")
				echo '<option selected="selected" value="1">Wh</option>';
			else
				echo '<option value="1">Wh</option>';

			if($units == "0.001")
				echo '<option selected="selected" value="0.001">KWh</option>';
			else
				echo '<option value="0.001">KWh</option>';

			if($units == "0.000001")
				echo '<option selected="selected" value="0.000001">MWh</option>';
			else
				echo '<option value="0.000001">MWh</option>';

			echo '</select>';
			echo ' &nbsp; <button name="change">Change</button>';
			echo '</form>';

			echo '</div>';

		}

		return $units;
	}
	
	
	public function isDNorCN($customer_id, $year, $month){
		//

		return 1;
	}

	
	protected function receiptStatic(){
		return array(
			"optic"=>array(4, "Optic Fibre"),
			"energy"=>array(3, "Energy Sales"),
		);
	}
	
	protected function deletor($table, $column,  $id, $return_to){
		$db = new Db();
		
		$id = portion(3);
		$sql = "DELETE FROM $table WHERE $column = '$id'";
		
		AuditTrail::registerTrail($sql, $id,  $table, $column);

		$delete = $db->query($sql);
		
		if($delete){
			FeedBack::success('Deleting. Please wait ...');
			FeedBack::refresh(1,return_url().$return_to);
		}else{
			FeedBack::error('Not Deleted '.$db->error());
		}
	}
	

	public function defaultUnits($id){
		if($id==1)
			return "Wh";
		if($id == 2)
			return "KWh";
		if($id == 3) 
			return "MWh";

		return "";
	}

	public function isLocked($customer_id, $year, $month){
		$db = new Db();

		$select = $db->select("SELECT * FROM lock WHERE lock_customer_id = '$customer_id' AND lock_year = '$year' AND lock_month='$month' AND lock_status IS NOT NULL");

		if($db->num_rows()){
			return true;
		}else{
			return false;
		}

	}

	public function readingExist($customer_id, $year, $month){
		$db = new Db();
		$time_readings = strtotime(date($year.'-'.$month.'-15'));
		$sql = "SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id'";
		$select = $db->select($sql);
				
		if($db->num_rows()){
			return true;
		}else{
			return false;
		}
	}
	
	public function action($type, $link, $word = ""){
		
		$type = strtolower($type);
		
		if($type == "edit")
			return '<a href="'.return_url().$link.'" class="eagle-load btn btn-xs btn-success" style="margin:0 2px;">'.$word.'</a>';
		
		if($type == "delete")
			return '<a href="'.return_url().$link.'" class="btn btn-xs btn-danger" style="margin:0 2px;">'.$word.'</a>';
		
		if($type == "view")
			return '<a href="'.return_url().$link.'" class="eagle-load btn btn-xs btn-warning"> '.$word.'</a>';
			
		return 'none';
	}
	public function before(){
	}
	public function before1(){
		
		$link_part0 = str_replace('-', '-', ucwords(portion(1)));
		$link_part5 = str_replace(' ', '',ucwords(str_replace('-', ' ', portion(1))));
		$link_part1 = portion(2);
		
		$link_part0 = (empty($link_part0))?"Dashboard":$link_part0;
		$link_part5 = (empty($link_part5))?"Dashboard":$link_part5;
		$link_part1 = (empty($link_part1))?"index":$link_part1;
		
		$title = ucwords(strtolower(str_replace('-', ' ', $link_part1)));
		echo '<div class="page-header" style="margin:0;margin-top:-10px;color:black;font-size: 1.2em; font-weight: bold;text-transform:uppercase; "><span style="">'.str_replace(' Of ',' of ',$title) .'</span>';
		
		echo '<ul class="inner-menu">';
				
		$class_name = ucwords($link_part5);
		$h = new $class_name;
		
		if(!empty($h->getLinks())){

			foreach($h->getLinks() as $link){
				extract($link);								
				$a = new AccessRights();
				if($a->sectionAccess(user_id(), $link_page, $link_right))
				{				
					// echo '<li>';
					
					// if(strtolower($link_part0).'/'.$link_part1 == $link_address)
					// 	echo '<i class="fa fa-fx '.$link_icon.'"></i> '.$link_name;
					// else
					// 	echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
					
					// echo '</li>';
				}
			}
		}
		
		echo '</ul>';
		echo '</div>';
	}
	
	public function full_name($id){
		$db = new Db();
		$row = $db->select("SELECT user_surname, user_othername FROM sysuser WHERE user_id = '$id'");
		$db->error();
		return ucwords(strtolower($row[0][0]['user_surname'].' '.$row[0][0]['user_othername']));
	}
	//return user field
	protected function ruf($id, $column){
		$db = new Db();
		$row = $db->select("SELECT $column FROM sysuser WHERE user_id = '$id'");
		echo $db->error();
		return $row[0][0][$column];
	}
	//return generic field from generic table
	public function rgf($table, $id, $look_up, $column){
		$db = new Db();
		$sql = "SELECT $column FROM $table WHERE $look_up = '$id' ";
		$row = @$db->select($sql);
		$db->error();
		
		return @$row[0][0][$column];
	}

	//return distinct field from generic table
	protected function rdfs($table, $column, $order=array()){
		$db = new Db();
		$row = $db->select("SELECT distinct $column FROM $table ");
		echo $db->error();
		return $row[0];
	}

	protected function hod($id){
		$db = new Db();
		$row = $db->select("SELECT user_department_id FROM sysuser WHERE user_id = '$id'");
		$user_department_id = $row[0][0]['user_department_id'];
		$hod = static_hod_id();
		$b = new Db();
		$bb = $b->select("SELECT user_id FROM sysuser WHERE user_designation = '$hod' AND user_department_id = '$user_department_id'");					
		extract($bb[0][0]);
		$user_id;
		return $user_id;
	}
	protected function dept_name($id){
		$db = new Db();
		$row = $db->select("SELECT dept_name FROM department WHERE dept_id = '$id'");
		echo $db->error();
		return $row[0][0]['dept_name'];
	}
	
	protected function sec_name($id){
		$db = new Db();
		$row = $db->select("SELECT section_name FROM section WHERE section_id = '$id'");
		echo $db->error();
		return $row[0][0]['section_name'];
	}
	
	protected function hod_id($id){
		$db = new Db();
		$row = $db->select("SELECT hod_user_id FROM hod WHERE hod_dept_id = '$id'");
		echo $db->error();
		return @$row[0][0]['hod_user_id'];
	}

	public function tou($customer_id, $month, $year, $note=0){
		$db = new Db();

		if($note)
			$time_band = "time_band_cn";
		else
			$time_band = "time_band";

		$select = $db->select("SELECT * FROM $time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id DESC");
		$assigned = 0;
		
		if($assigned == 0){
			foreach($select[0] as $row){
				extract($row);
				if($tb_year==$year && $tb_month==$month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$narration = $tb_narration;
					$assigned = 1;
					break;
				}
			}

		}
		
		if($assigned == 0){
			foreach($select[0] as $row){
				extract($row);
				if(date('Y', $tb_date_added)==$year && date('m', $tb_date_added)==$month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$narration = $tb_narration;
					$assigned = 1;
					break;
				}
			}
		}

		if($assigned == 0){
			$db = new Db();
			$select = $db->select("SELECT * FROM $time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id DESC");
			$assigned = 0;
			foreach($select[0] as $row){
				extract($row);
				if(date('Y', $tb_date_added)<=$year && date('m', $tb_date_added) <= $month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$narration = $tb_narration;
					$assigned = 1;
					break;
				}
			}
		}

		if($assigned == 0){
			$db = new Db();
			$select = $db->select("SELECT * FROM $time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id DESC");
			$assigned = 0;
			foreach($select[0] as $row){
				extract($row);
				//if(date('Y', $tb_date_added)<=$year && date('m', $tb_date_added) <= $month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$narration = $tb_narration;
					$assigned = 1;
					break;
				}
			}
		}

		return array($peak, $shoulder, $off_peak, $effective_date, $expiry_date, $narration);
	}
	
	protected function total($table, $column="", $column_value=""){
		//echo "SELECT * FROM $table WHERE $column = '$column_value'";
		$db = new Db();
		if(empty($column))
			$row = $db->select("SELECT * FROM $table");
		else
			$row = $db->select("SELECT * FROM $table WHERE $column = '$column_value'");
			
		echo $db->error();
		return @$db->num_rows();
	}
	
	
	protected function branch_name($id){
		$db = new Db();
		$row = $db->select("SELECT branch_name FROM branch WHERE branch_id = '$id'");
		echo $db->error();
		return $row[0][0]['branch_name'];
	}
	
	protected function role_id($id){
		$db = new Db();
		$row = $db->select("SELECT users.role_id as role_id FROM users, roles WHERE users.user_id = $id AND users.role_id = roles.role_id limit 1");
		
		return $row[0]['role_id'];
	}
	
	protected function role_name($id){
		$db = new Db();
		$row = $db->select("SELECT role_name FROM users, roles WHERE users.user_id = $id AND users.role_id = roles.role_id limit 1");
		
		return $row[0]['role_name'];
	}
	
	protected function designation_name($id){
		$db = new Db();
		$row = $db->select("SELECT designation_name FROM designation WHERE designation_id = '$id'");
		echo $db->error();
		return $row[0][0]['designation_name'];
	}

	public function next($level, $year, $month){
		if($level == 3){
			$level = 1;
		}else{
			$level = $level+1;
		}
		$db = new Db();
		$select = $db->select("SELECT dl_selected".($level)." as did FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' ");
		extract($select[0][0]);

		$user_id = $this->rgf("sysuser", $did, "user_designation", "user_id");

		$fname = $this->rgf("sysuser", $user_id, "user_id", "user_surname");
		$sname = $this->rgf("sysuser", $user_id, "user_id", "user_othername");
		$email = $this->rgf("sysuser", $user_id, "user_id", "user_email");

		return array('fname'=>$fname, 'sname'=>$sname, 'email'=>$email);

	}

		
	public function rejector($table, $column_name=array(), $column_prefix=array()){
		$u = new Db();
		
		foreach ($column_name as $key => $value) {
			$column_name_and_value = $key.'='.$value.'';
		}
		
		foreach ($column_prefix as $key => $value) {
			$column_prefix_db = $key;
			$column_count = $value;
		}

		for($i=0; $i<$column_count-1; $i++){
			$lev[] = ' '.$column_prefix_db.($i+1)." = NULL ";
		}
		$levs = implode(" , ", $lev);

		$sql = "UPDATE $table SET $levs WHERE $column_name_and_value";

		$db = new Db();
		$update = $db->query($sql);
	}

	protected function department($id, $table = false){
		$db = new Db();
		

		if($table==true){
			$row = $db->select("SELECT dp_name FROM department WHERE dp_id = '$id'");
		}else{
			$row = $db->select("SELECT dp_name FROM users, department WHERE users.user_id = $id AND users.user_dp_id = department.dp_id limit 1");
		}
		
		return $row[0]['dp_name'];
	}
	
	protected function check_de_status($user_id, $type="SMS"){
		//$db = new Db();
		//SELECT de_id, de_status, de_name, de_user_id FROM disable_enable WHERE 1
		//$row = $db->select("SELECT de_status FROM disable_enable WHERE de_user_id = $user_id AND de_name = '$type' limit 1");
		
		//if($db->num_rows()==0){
			//return 1;
		//}

		//return !($row[0]['de_status']);
		return 1;
	}

		
	public function department_id($id){
		$db = new Db();
		$row = $db->select("SELECT user_dp_id FROM users WHERE users.user_id = $id limit 1");
		
		return @$row[0]['user_dp_id'];
	}

	protected function isThere($table, $params = array()){
		$db = new Db();

		foreach ($params as $key => $value) {
			$vals[] = "$key='$value'";
		}

		$all_vals = implode(' AND ', $vals);

		$select = $db->select("SELECT * FROM $table WHERE $all_vals;");

		if($db->num_rows()){
			return true;
		}else{
			return false;
		}

	}

	protected function serviceType($id){
		if($id == 1)
			return "Energy";
			
		return "Opic Fibre";
	}
	protected function isThereEdit($table, $params = array()){
		
		$db = new Db();
		$i=0;
		$all = count($params);

		foreach ($params as $key => $value) {
			$i++;
			
			if($all == $i){
				$vals[] = "$key != '$value'";
			}else{
				$vals[] = "$key = '$value'";
			}
		}

		$all_vals = implode(' AND ', $vals);
		//echo "SELECT * FROM $table WHERE $all_vals;";
		$select = $db->select("SELECT * FROM $table WHERE $all_vals;");

		if($db->num_rows()){
			return true;
		}else{
			return false;
		}

	}

	protected function slashValuesTrim(&$params){
		foreach($params as &$var){
			is_array($var)? slashValues($var) : $var = trim(addslashes($var));
			unset($var);
		}
	}


	protected function stripValues(&$params){
		foreach($params as &$var){
			is_array($var)? html($var) : $var = trim(stripslashes($var));
			unset($var);
		}
	}
	
	

	public function period($date){

		$mo = array(
			"JAN"=>"07",
			"FEB"=>"08",
			"MAR"=>"09",
			"APR"=>"10",
			"MAY"=>"11",
			"JUN"=>"12",
			"JUL"=>"01",
			"AUG"=>"02",
			"SEP"=>"03",
			"OCT"=>"04",
			"NOV"=>"05",
			"DEC"=>"06", 
		);
		$p = strtotime("+6 months", $date);
		return str_pad(date("m", $p),3,0).'/'.str_pad(date("Y", $p),4,0);
	}

	protected function after(){
		
	}

	public function invoiceCreator($date, $months){
		$months_to_use = "+$months months";
		$periods = array();
		$date1 = date('Y-m-d', strtotime($months_to_use, $date));
		$date2 = (strtotime("-1 day", strtotime($date1)));
		
		$periods['start_date'] = Feedback::date_s($date);
		$periods['end_date'] = Feedback::date_s($date2);
		$periods['next_start'] = $date1;

		return $periods;
	}

	public function notified($id, $contract_id=0){
		$db = new Db();
		$select = $db->select("SELECT * FROM notification WHERE not_customer_id = '$id' AND not_contract_id = '$contract_id'");		
		$sd = array();
		if($db->num_rows()){
			foreach($select[0] as $row){
				extract($row);
				$sd[] = $not_start_date;
			}
		}


		return $sd;
	}

	public function startNotification(){
		$db = new Db();
		$select = $db->select("SELECT TOP 1 fob_year, fob_month FROM fibre_opening_balance ORDER BY fob_id DESC");
		extract($select[0][0]);
		return $start = strtotime("$fob_year-$fob_month-01");
	}

	public function notify($customer_id, $status=1){
		$db = new Db();
		$sql = "SELECT contract_id FROM contract WHERE contract_ofc_id = '$customer_id'";
		$sel = $db->select($sql);
		$count = 0;
		foreach($sel[0] as $r){			
			extract($r);

			$days = (40*24*60*60);//+days
			$time_now = (time()+$days);//+days
			$terminated = ($this->isTerminated($contract_id))?$this->isTerminated($contract_id):(time()+$days);

			$select = $db->select("SELECT TOP 1 * FROM fibre_service WHERE fs_customer_id = '$customer_id' AND fs_contract = '$contract_id'");

			if($db->num_rows()){
				extract($select[0][0]);
				$ed = $fs_effective_date;
				$q = $fs_qty;

				$db = new Db();
				$sd = $this->notified($customer_id, $fs_contract);
				
				while($ed < $time_now){
					$p = $this->invoiceCreator($ed, $q);
					if($ed < $terminated){
						$start_date = strtotime($p['start_date']);
						if($status){
							if(!in_array($start_date, $sd) && $start_date > $this->startNotification()){
								if($start_date > $start && $start_date >= strtotime('2024-01-10')) $count++;
							}
						}else{
							if($start_date > $start  && $start_date >= strtotime('2024-01-10')) $count++;
						}
					}
					$ed = strtotime($p['next_start']);
				}
			}else{
				
			}
		}

		return $count;
	}

	public function emailNotify(){
		$db = new Db();
		$count = 0;
		$select = $db->select("SELECT TOP 1 fob_year, fob_month FROM fibre_opening_balance ORDER BY fob_id DESC");
		extract($select[0][0]);
		$start = strtotime("$fob_year-$fob_month-01");

		$select = $db->select("SELECT * FROM fibre_service, of_customer WHERE ofc_id = fs_customer_id");
		$x=array();
		$check = array();

		foreach($select[0] as $row){

			extract($row);
			$ed = $fs_effective_date;
			$q = $fs_qty;

			$sd = $this->notified($fs_customer_id, $fs_contract);
			
			while($ed < (time()+7*24*60*60)){
				$p = $this->invoiceCreator($ed, $q);
				$start_date = strtotime($p['start_date']);
				$ed = strtotime($p['next_start']);
				$end_date = strtotime($p['end_date']);
				if(!in_array($start_date, $sd) && $start_date > $start)
				{
					$std = FeedBack::date_r($start_date);
					$end = FeedBack::date_r($end_date);
					if(!in_array($ofc_id.$start_date, $check))
					{
						$check[] = $ofc_id.$start_date;
						
						//create a table for customers
						//$x[] = ''.$ofc_customer_name.' <b>('.$std.' - '.$end.')</b> <a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'">You can use this link to View Invoice</a>';

						$x[] = '<a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'">'.$ofc_customer_name.' <b>('.$std.' - '.$end.')</b></a>';
					}
				}
			}
		}

		if(!empty($x)){
			$to = "josemusiitwa@gmail.com";
			$subject = "UETCL OPTIC FIBRE INVOICE(S)";
			
		
			$message .= "\r\n Hello <b>Joseph</b>,\r\n";
			$message .= "\r\n You have the following invoice to view and send/print\r\n";
			$message .= "\r\n<ol><li>".implode("</li><li>", $x)."</li></ol>";
			
			//FeedBack::sendmail($to,$subject,$message);
		}
		return ($message);
	}

	public function notifyAll(){
		$db = new Db();
		$n = 0;
		$select = $db->select("SELECT ofc_id FROM of_customer");
		foreach($select[0] as $row){
			extract($row);
			$n += $this->notify($ofc_id);
		}

		return $n;
		// $count = 0;
		// $select = $db->select("SELECT TOP 1 fob_year, fob_month FROM fibre_opening_balance ORDER BY fob_id DESC");
		// extract($select[0][0]);
		// $start = strtotime("$fob_year-$fob_month-01");

		// $select = $db->select("SELECT * FROM fibre_service, of_customer WHERE ofc_id = fs_customer_id");
		// $check = array();
		// foreach($select[0] as $row){
		// 	extract($row);
		// 	$ed = $fs_effective_date;
		// 	$q = $fs_qty;

		// 	$sd = $this->notified($fs_customer_id, $fs_contract);

		// 	notify
			
		// 	while($ed < (time()+7*24*60*60)){
		// 		$p = $this->invoiceCreator($ed, $q);
		// 		$start_date = strtotime($p['start_date']);
		// 		$ed = strtotime($p['next_start']);

		// 		if(!in_array($start_date, $sd) && $start_date > $start)
		// 		{
		// 			if(!in_array($ofc_id.$start_date, $check))
		// 			{
		// 				$check[] = $ofc_id.$start_date;
		// 				$count++;
		// 			}
		// 		}
		// 	}
		// }

		//return $count;
	}
public function getOrdinalDate($timestamp) {
    $day = date('j', $timestamp); // Day without leading zero
    $suffix = '';

    if ($day % 10 == 1 && $day != 11) {
        $suffix = 'st';
    } elseif ($day % 10 == 2 && $day != 12) {
        $suffix = 'nd';
    } elseif ($day % 10 == 3 && $day != 13) {
        $suffix = 'rd';
    } else {
        $suffix = 'th';
    }

    return $day . $suffix;
}	
public function GL_ACCOUNT($invoice_no, $post_date,$invoice_date,$cust_id,$entity, $jv_details,$currency){
	if($entity=="ENERGY"){
	$db = new Db();	
	$sql = "SELECT * FROM customer where customer_id = '$cust_id'";
	$select =  $db->select($sql);
	if($db->num_rows()){
		extract($select[0][0]);
	 }
	 $cust_name = $customer_full_name;
	 $remarks = "Energy sales";
	 if($customer_type=="B2F"){
	 	 $exchange_rate = $this->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_exchange_rate");	
	 }
	}
	if($entity=="FIBRE"){
	$db = new Db();	
	$sql = "SELECT * FROM of_customer where ofc_id = '$cust_id'";
	$select =  $db->select($sql);
	if($db->num_rows()){
		extract($select[0][0]);
	 }
	 $cust_name = $ofc_customer_name;
	 $remarks = "";
	 //$this->rgf("services_fibre",$fs_description,"service_id","service_name")
	 
	 if($ofc_cust_type=="B2F"){
	 	 $exchange_rate = $this->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_exchange_rate");	
	 }
	}
	if($entity=="OTHERINVOICES"){
      $db=new Db();
		$select=$db->select("SELECT * from customers WHERE cust_id='$cust_id'");
		if($db->num_rows()){
		extract($select[0][0]);
	    }
	     $cust_name = $cust_name;
	     $remarks =$this->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_general_description");
	     if($currency=='102'){
	 	 $exchange_rate = $this->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_exchange_rate");	
	   }	      
	}
	if($entity=="PURCHASES"){
      $db=new Db();
		$select=$db->select("SELECT * from generators WHERE ge_id='$cust_id'");
		if($db->num_rows()){
		extract($select[0][0]);
	    }
	     $cust_name = $ge_name;
	     $remarks ="Energy Purchase";
	    	      
	}
	if($currency=='101'){
		$cur = "UGX";
	}elseif($currency=='102'){
		$cur = "USD";
	}elseif($currency=='104'){
		$cur = "EUR";
	}
	//echo "@@@@@@@@".$exchange_rate;
	extract($jv_details[0]);


	$db = new Db();	
	$sql1 = "SELECT * FROM gl_account where gl_cust_id = '$cust_id' AND gl_entity='$entity'";
	$select1 =  $db->select($sql1);
	if($db->num_rows()){
		extract($select1[0][0]);

		// print_r($select1);
	 }

	?>
	<style>
		#jv td{
			padding:2px 5px !important;
		}
	</style>
	<table id="jv" border="0" style="font-size:12px; font-family:arial border:none;" cellpadding="1" cellspacing="0" width="100%">
    <tr>

        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td colspan="9"><b><?php echo $invoice_no;?></b></td>
        
    </tr>
    <tr>
        <td colspan="28" style="text-align: center;"><b>UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</b></td>

    </tr>
    <tr>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td colspan="28" style="text-align: center;font-weight: bold;">COST JOURNAL VOUCHER</td>
       
    </tr>
    <tr>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
    </tr>
    <tr>

        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;border-top:1px solid black;"><?php echo $this->getOrdinalDate($post_date);?></td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;border-top:1px solid black;"><?php echo date('M',$post_date);?></td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;border-top:1px solid black;"><?php echo date('y',$post_date);?></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>

        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">DD</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">MM</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">YY</td>
        <td></td>
        <td colspan="2" style="font-size: 8px;font-weight: bold;">TRANSACTION TYPE</td>
      
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td colspan="28">NARRATION: <span style="border-bottom: 1px solid black;"><?php echo $remarks;?> To <?php echo $cust_name;?> during the month of <?php echo date('M Y',$invoice_date);?></span></td>
    </tr>
    <tr>
    
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td colspan="3" style="border-left:1px solid black;border-top:1px solid black;border-right:1px solid black;border-bottom:1px solid black;text-align: center;">PERIOD</td>
        <td style="border-right:1px solid black;border-top:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-left:1px solid black;border-top:1px solid black;">&nbsp;</td>
        <td style="border-top:1px solid black;">&nbsp;</td>
        <td style="border-top:1px solid black;">&nbsp;</td>
        <td style="border-top:1px solid black;">&nbsp;</td>
        <td style="border-top:1px solid black;">&nbsp;</td>
        <td style="border-top:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td colspan="2" style="border-left:1px solid black;border-right:1px solid black;border-top:1px solid black;text-align: center;">AMOUNT (<?php echo $cur;?>)</td>
        <td></td>
        <td colspan="7" style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;border-top:1px solid black;text-align: center;">COST CENTRE</td>
        
        <td></td>
        <td colspan="4" style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;border-top:1px solid black;text-align: center;">VEHICLE/CHECK NO</td>
        <td></td>
    </tr>
    <tr>
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">DD</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">MM</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">YY</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;text-align: center;">PARTICULARS</td>
        <td></td>
        <td colspan="6" style="border-left:1px solid black;border-right:1px solid black;text-align: center;border-bottom:1px solid black;">GL ACCOUNT</td>
        
        <td></td>
        <td style="border-top:1px solid black;border-right:1px solid black;border-left:1px solid black;">DR</td>
        <td style="border-top:1px solid black;border-right:1px solid black;">CR</td>
        <td></td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;border-left:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">F</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">A</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">0</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">3</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>

        <td></td>
    </tr>

	<!-- dynamic starts -->

	<?php 
		$i = 0;

		$credits = array();
		$debits = array();

		foreach($select1[0] as $row){
			// print_r($row);
			extract($row);
			$gl_account = ( $this->rgf("dictionary", $gl_account, "d_id", "d_code")); 
			$gl_account = str_split($gl_account); 


			$gl_label =  $this->rgf("dictionary", $gl_label, "d_id", "d_code");

			$amount = "";
			if(strtolower($gl_label) == strtolower("Off Peak"))
				$amount = $oi_off_peak;
			else if(strtolower($gl_label) == strtolower("Peak"))
				$amount = $oi_peak;
			else if(strtolower($gl_label) == strtolower("Shoulder"))
				$amount = $oi_shoulder;
			else if(strtolower($gl_label) == strtolower("VAT"))
				$amount = $oi_vat_amount;
			else if(strtolower($gl_label) == strtolower("Wheeling Charges - Collectable"))
				$amount = $oi_collectable;
			else if(strtolower($gl_label) == strtolower("Wheeling Charges - Payable"))
				$amount = $oi_payable;
			
			else if(strtolower($gl_label) == strtolower("Net"))
			$amount = $oi_net;

			else if(strtolower($gl_label) == strtolower("TOTAL AMOUNT (VAT INCL)"))
			$amount = $oi_gross;


			$row = '<tr>';

			if($i==0){

				$row .= '<td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">'.$this->getOrdinalDate($invoice_date).'</td>';
        		$row .= '<td style="border-right:1px solid black;border-bottom:1px solid black;">'.date('M',$invoice_date).'</td>';
        		$row .= '<td style="border-right:1px solid black;border-bottom:1px solid black;">'.date('y',$invoice_date).'</td>';
        		$row .= '<td style="border-right:1px solid black;">'.$remarks.' to '.$cust_name.' during the Month of '.date('M Y',$invoice_date).'  Ref. Inv. '.$invoice_no.'</td>';

			}else{
				$row .= '<td style="border-left:1px solid black;border-right:1px solid black;">&nbsp;</td>';
				$row .= '<td style="border-right:1px solid black;">&nbsp;</td>';
				$row .= '<td style="border-right:1px solid black;">&nbsp;</td>';
				$row .= '<td style="border-right:1px solid black;">&nbsp;</td>';
			}

			$row .= '<td></td>';

			$k = 0;
			foreach($gl_account as $number){
				if($k==0)
				$row .= '
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">'.$number.'</td>';
				else
					$row .= '<td style="border-right:1px solid black;border-bottom:1px solid black;">'.$number.'</td>';

				$k++;
			}

			$row .= '<td></td>';

			if($gl_ct_bt == "Credit"){
				$row .= '<td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">&nbsp;</td>';
				$row .= '<td style="border-top:1px solid black;border-right:1px solid black;border-bottom:1px solid black; text-align:right;">'.number_format($amount, 2).' </td>';
				$credits[] = $amount;
			}else{
				$row .= '<td style="border-top:1px solid black;border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">'.number_format($amount, 2).' </td>';
				$row .= '<td style="border-top:1px solid black;border-right:1px solid black;border-left:1px solid black;">&nbsp;</td>';		
				
				$debits[] = $amount;		
			}

			$row .= '<td></td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;border-left:1px solid black;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>';
			$row .= '<td></td>';
			$row .= '<td style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>';
			$row .= '<td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>';

			$row .= '</tr>';

			
			echo $row;
			$i++;
		}

	?>

    <tr>
        <td style="border-left:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td  style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td></td>
        <td colspan="5" style="border-bottom:1px solid black;border-right:1px solid black;border-left:1px solid black;"><?php if($exchange_rate) echo 'Exch Rate:USD:UGX '.number_format($exchange_rate,2);?></td>
        
       
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td></td>
    </tr>
    <tr>
        <td style="border-left:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td  style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;border-left:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td></td>
    </tr>
    
    <tr>

        <td style="border-left:1px solid black;border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td></td>
        <td  style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-right:1px solid black;border-left:1px solid black;border-bottom:1px solid black;text-align:right;"><b> <?php echo number_format(array_sum($debits), 2); ?> </b></td>
        <td style="border-right:1px solid black;border-bottom:1px solid black;text-align:right;"><b><?php echo number_format(array_sum($credits), 2); ?></b></td>
        <td></td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;border-left:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;">&nbsp;</td>
        <td></td>
        <td style="border-bottom:1px solid black;border-left:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td style="border-bottom:1px solid black;border-right:1px solid black;text-align: center;">&nbsp;</td>
        <td></td>
    </tr>
    <tr>
 
        <td colspan="3">DATA FORM PREP. BY:</td>
        <td> VERIFIED BY: HC</td>
        <td></td>
        <td colspan="6"> APPROVED BY: MBFO</td>
        <td></td>
        <td colspan="2">APPROVED BY: HF</td>
        <td></td>
        <td colspan="2">DATA ENTERED BY:</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td>POSTED BY: </td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    
    <tr>
     
        <td colspan="3">DATE:</td>
        <td>DATE:</td>
        <td></td>
        <td colspan="6">DATE:</td>
        <td></td>
        <td colspan="2">DATE:</td>
        <td></td>
        <td colspan="2">DATE:</td>
        <td></td>
        <td>JV REF NO.</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td>REF:</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    
</table>
<?php
}

}
//$start_date > 1580334904
//1=1

?>