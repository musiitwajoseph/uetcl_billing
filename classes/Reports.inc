<?php

Class Reports extends BeforeAndAfter{
	public $page = "REPORTS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "REPORTS";
		$links = array(
			array(
				"link_name"=>"Optic Fibre Data", 
				"link_address"=>"reports/optic-fibre-data",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Regional Sales", 
				"link_address"=>"reports/regional-sales",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Purchases and Sales", 
				"link_address"=>"reports/purchases-and-sales",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Transmission Losses", 
				"link_address"=>"reports/transmission-losses",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"TOU Breakdown", 
				"link_address"=>"reports/tou-breakdown",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Optic Fibre Balance", 
				"link_address"=>"reports/balance",
				"link_icon"=>"fa-money",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}

	public function purchasesAndSales(){
				?>
<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
					
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{
				$year = strtotime('-1 month', time());

				$month_uploaded = (portion(4))? portion(4) : date('m', $year);
				$year_uploaded = (portion(3))? portion(3) : date('Y', $year);
			}
			
			?>
			
		</form>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=$fob_year; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>

		<?php

		$year = $year_uploaded;
		$month = $month_uploaded;

		$time = strtotime("$year-$month-01");
		$prev = strtotime('-1 month', $time);

		$prev_year = date('Y', $prev);
		$prev_month = date('m', $prev);

		$time = strtotime("-1 year", $time);
		$pyear = date('Y', $time);
		$pmonth = date('m', $time);
		
		$prev = strtotime('-1 month', $time);
		$pprev_year = date('Y', $prev);
		$pprev_month = date('m', $prev);
		echo '<span onclick="return print(\'printMe\');" class="pull-right btn btn-default"><i class="fa fa-print"></i> Print</span>';
		echo '<div id="side">';
		echo '<div class="clearfix"/></div>';
		echo '<div id="printMe">';
		//echo '<style>@media print{@page {size: landscape}</style>';

		//echo '<h5><center class="pull-left">STATEMENT OF TRANSMISSION VARIANCE FOR '.strtoupper(month_name($month)).' '.$year.'</center></h5>';

		echo '<div class="clearfix"/></div>';
		
		echo '<h5>Purchases and Sales</h5>';
		echo '<table id="table" cellspacing="0" cellpadding="2" border="1" style="margin-left:0;width:auto;font-size:8px;">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="4"><center>No</center></th>';
		echo '<th rowspan="4"><center>GENERATOR<br/>/ Purchases</center></th>';
		echo '</tr>';
		echo '<tr>';
		echo '<th colspan="6"><center>ENERGY PURCHASE UNITS </center></th>';		
		echo '<th style="border-top:none; border-bottom:none; background-color:white; width:2px;"></th>';
		echo '<th colspan="6"><center>ENERGY PURCHASE VALUE </center></th>';

		echo '<th style="border-top:none; border-bottom:none; background-color:white;width:12px;"></th>';
		//======================== AGO ============================
		echo '<th colspan="5"><center>% Changes</center></th>';

		echo '</tr>';
		echo '<tr>';
		//echo '<th rowspan="2"><center>Category</center></th>';
		echo '<th><center>'.month_name_short($month).'\''.date('y', strtotime("$year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($prev_month).'\''.date('y', strtotime("$prev_year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($pmonth).'\''.date('y', strtotime("$pyear-01-01")).'</center></th>';
		//echo '<th><center>'.month_name_short($pprev_month).'\''.date('y', strtotime("$pprev_year-01-01")).'</center></th>';

		echo '<th><center>'.month_name_short($month).'\''.date('y', strtotime("$year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($prev_month).'\''.date('y', strtotime("$prev_year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($pmonth).'\''.date('y', strtotime("$pyear-01-01")).'</center></th>';
		//echo '<th><center>'.month_name_short($pprev_month).'\''.date('y', strtotime("$pprev_year-01-01")).'</center></th>';

		echo '<th style="border-top:none; border-bottom:none; background-color:white;"></th>';

		echo '<th><center>'.month_name_short($month).'\''.date('y', strtotime("$year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($prev_month).'\''.date('y', strtotime("$prev_year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($pmonth).'\''.date('y', strtotime("$pyear-01-01")).'</center></th>';
		//echo '<th><center>'.month_name_short($pprev_month).'\''.date('y', strtotime("$pprev_year-01-01")).'</center></th>';

		echo '<th><center>'.month_name_short($month).'\''.date('y', strtotime("$year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($prev_month).'\''.date('y', strtotime("$prev_year-01-01")).'</center></th>';
		echo '<th><center>'.month_name_short($pmonth).'\''.date('y', strtotime("$pyear-01-01")).'</center></th>';
		//echo '<th><center>'.month_name_short($pprev_month).'\''.date('y', strtotime("$pprev_year-01-01")).'</center></th>';

		echo '<th style="border-top:none; border-bottom:none; background-color:white;"></th>';
		//======================== AGO ============================
		echo '<th colspan="2">Units</th>';
		echo '<th></th>';
		echo '<th colspan="2">Financials</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th><center>GWh</center></th>';
		echo '<th><center>GWh</center></th>';
		echo '<th><center>GWh</center></th>';
		//echo '<th><center>GWh</center></th>';

		echo '<th><center>%</center></th>';
		echo '<th><center>%</center></th>';
		echo '<th><center>%</center></th>';
		//echo '<th><center>%</center></th>';
		echo '<th style="border-top:none; border-bottom:none; background-color:white;"></th>';
		echo '<th><center>Bn</center></th>';
		echo '<th><center>Bn</center></th>';
		echo '<th><center>Bn</center></th>';
		//echo '<th><center>Bn</center></th>';

		echo '<th><center>%</center></th>';
		echo '<th><center>%</center></th>';
		echo '<th><center>%</center></th>';
		//echo '<th><center>%</center></th>';


		//====================== AGO ===================================
		echo '<th style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;"></th>';

		echo '<th style="text-align:right; font-weight:bold;">Yago</th>';
		echo '<th style="text-align:right; font-weight:bold;">Mago</th>';

		echo '<th style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;width:2px;"></th>';

		echo '<th style="text-align:right; font-weight:bold;">Yago</th>';
		echo '<th style="text-align:right; font-weight:bold;">Mago</th>';


		echo '</tr>';
		$db = new Db();
		$no = 1;
		$tc = array();
		$tp = array();
		$ptc = array();
		$ptp = array();

		$ac = array();
		$ap = array();
		$pac = array();
		$pap = array();

		$select = $db->select("SELECT ge_id, mix_id, mix_units, mix_amount, mix_amount2, mix_amount3 FROM generators,mix_price WHERE mix_year = '$year' AND mix_month = '$month' AND mix_generator_id = ge_id ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);
			$tc[$ge_id] = $mix_units;
			$ac[$ge_id] = $mix_amount3;
		}

		$select = $db->select("SELECT ge_id, mix_id, mix_units, mix_amount, mix_amount2, mix_amount3 FROM generators,mix_price WHERE mix_year = '$prev_year' AND mix_month = '$prev_month' AND mix_generator_id = ge_id ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);
			$tp[$ge_id] = $mix_units;
			$ap[$ge_id] = $mix_amount3;
			$mix_amount3;
		}
		$select = $db->select("SELECT ge_id, mix_id, mix_units, mix_amount, mix_amount2, mix_amount3 FROM generators,mix_price WHERE mix_year = '$pyear' AND mix_month = '$pmonth' AND mix_generator_id = ge_id ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);
			$ptc[$ge_id] = $mix_units;
			$pac[$ge_id] = $mix_amount3;
		}

		$select = $db->select("SELECT ge_id, mix_id, mix_units, mix_amount, mix_amount2, mix_amount3 FROM generators,mix_price WHERE mix_year = '$pprev_year' AND mix_month = '$pprev_month' AND mix_generator_id = ge_id ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);
			$ptp[$ge_id] = $mix_units;
			$pap[$ge_id] = $mix_amount3;
		}


		$ttc = array_sum($tc);
		$ttp = array_sum($tp);
		$pttc = array_sum($ptc);
		$pttp = array_sum($ptp);

		$tac = array_sum($ac);
		$tap = array_sum($ap);
		$tpac = array_sum($pac);
		$tpap = array_sum($pap);

		$cus = array();
		$db = new Db();
		$q = 1000000000;//GWh
		$b = 1000000000;

		$uyago = array();
		$umago = array();
		$fyago = array();
		$fmago = array();	

		$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);

			echo '<tr>';
			echo '<td style="'.$b.'">'.($no++).'.</td>';
			echo '<td style="'.$b.'">'.$ge_name.'</td>';
			//echo '<td style="'.$b.'">'.$this->rgf('generator_category', $ge_category, 'gc_id', 'gc_name').'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tc[$ge_id]/$q,2).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tp[$ge_id]/$q,2).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($ptc[$ge_id]/$q,2).'</td>';
			//echo '<td style="text-align:right;'.$b.'">'.number_format($ptp[$ge_id]/$q,2).'</td>';

			$l = ($tc[$ge_id] - $ptc[$ge_id])/$ptc[$ge_id]*100;
			$ll = ($tc[$ge_id] - $tp[$ge_id])/$tp[$ge_id]*100;

			echo '<td style="text-align:right;'.$b.'">'.number_format($tc[$ge_id]/$ttc*100, 1).' %</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tp[$ge_id]/$ttp*100, 1).' %</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($ptc[$ge_id]/$pttc*100, 1).' %</td>';
			//echo '<td style="text-align:right;'.$b.'">'.number_format($ptp[$ge_id]/$pttp*100, 1).' %</td>';
				
			echo '<td style="text-align:right;'.$b.'; border-top:none; border-bottom:none; background-color:white;"></td>';

			echo '<td style="text-align:right;'.$b.'">'.number_format($ac[$ge_id]/$b,2).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($ap[$ge_id]/$b,2).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($pac[$ge_id]/$b,2).'</td>';
			//echo '<td style="text-align:right;'.$b.'">'.number_format($pap[$ge_id]/$b,2).'</td>';

			$al = ($ac[$ge_id] - $pac[$ge_id])/$pac[$ge_id]*100;
			$all = ($ac[$ge_id] - $ap[$ge_id])/$ap[$ge_id]*100;

			echo '<td style="text-align:right;'.$b.'">'.number_format($ac[$ge_id]/$tac*100, 1).' %</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($ap[$ge_id]/$tap*100, 1).' %</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($pac[$ge_id]/$tpac*100, 1).' %</td>';
			//echo '<td style="text-align:right;'.$b.'">'.number_format($pap[$ge_id]/$tpap*100, 1).' %</td>';


			//====================== AGO ===================================
			echo '<td style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;"></td>';

			echo '<td style="text-align:right; font-weight:bold;">'.number_format($l,2).' %</td>';
			echo '<td style="text-align:right; font-weight:bold;">'.number_format($ll,2).' %</td>';

			echo '<td style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;"></td>';

			echo '<td style="text-align:right; font-weight:bold;">'.number_format($al, 2).' %</td>';
			echo '<td style="text-align:right; font-weight:bold;">'.number_format($all, 2).' %</td>';

		

			echo '</tr>';
		}

		echo '<tr>';
		echo '<td></td>';
		echo '<td colspan="1" style="text-align:left; font-weight:bold;">Total Sales</td>';

		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ttc/$q,2).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ttp/$q,2).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($pttc/$q,2).'</td>';
		//echo '<td style="text-align:right; font-weight:bold;">'.number_format($pttp/$q,2).'</td>';


		$l = ($ttc[$ge_id] - $pttc[$ge_id])/$pttc[$ge_id]*100;
		$al = ($ttc[$ge_id] - $ttp[$ge_id])/$ttp[$ge_id]*100;

		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		//echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';

		echo '<td style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;"></td>';

		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tac/$q,2).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tap/$q,2).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tpac/$q,2).'</td>';
		//echo '<td style="text-align:right; font-weight:bold;">'.number_format($tpap/$q,2).'</td>';

		$al = ($ac[$ge_id] - $pac[$ge_id])/$pac[$ge_id]*100;
		$all = ($ac[$ge_id] - $ap[$ge_id])/$ap[$ge_id]*100;

		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		//echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';


		//====================== AGO ===================================
		echo '<td style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;"></td>';

		echo '<td style="text-align:right; font-weight:bold;">'.number_format($l, 2).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ll, 2).' %</td>';

		echo '<td style="text-align:right; font-weight:bold; border-top:none; border-bottom:none; background-color:white;"></td>';

		echo '<td style="text-align:right; font-weight:bold;">'.number_format($al, 2).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($all, 2).' %</td>';


		echo '</tr>';

		echo '</table>';



		echo '</div>';
		

	}

	public function balance(){
		echo '<h4>Optic Fibre Opening Balance</h4>';
		$db = new Db();
		echo '<form action="" method="post">';
		if(isset($_POST['save'])){
			$amount = $_POST['amount'];
			$customer = $_POST['customer'];
			$year = $_POST['year'];
			$month = $_POST['month'];

			for($i=0; $i<count($customer); $i++){
				$cus = $customer[$i];
				$select = $db->select("SELECT fob_id FROM fibre_opening_balance WHERE fob_customer_id = '$cus'");
				if($db->num_rows()){
					extract($select[0][0]);
					$db->update("fibre_opening_balance",[
						"fob_amount"=>str_replace(',','',$amount[$i]),
						"fob_year"=>$year,
						"fob_month"=>$month,
					],["fob_id"=>$fob_id]);
				}else{
					$db->insert("fibre_opening_balance",[
						"fob_amount"=>str_replace(',','',$amount[$i]),
						"fob_year"=>$year,
						"fob_month"=>$month,
						"fob_customer_id"=>$cus,
					]);
				}
			}
			echo $db->error();
			//FeedBack::redirect(return_url().'reports/balance');
		}
		$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
		echo '<input class="btn btn-primary btn-sm pull-right" type="submit" name="save" value="Save"/>';
		echo '<br/>';
		echo '<br/>';
		$s = $db->select("SELECT TOP 1 fob_year, fob_month FROM fibre_opening_balance");
		extract($s[0][0]);
		echo 'Year: <input type="text" autocomplete="off" value="'.$fob_year.'" placeholder="Enter Year" name="year"/>&nbsp;&nbsp;&nbsp;&nbsp;';
		echo 'Month: <input type="text" autocomplete="off" value="'.$fob_month.'" placeholder="Enter Month[1-12]" name="month"/>&nbsp;&nbsp;';
		echo '<table border="1" id="table">';
		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Customer Name</th>';
		echo '<th>Amount</th>';
		echo '</tr>';
		$no=1;
		foreach($select[0] as $row){
			extract($row);

			$sel = $db->select("SELECT * FROM fibre_opening_balance WHERE fob_customer_id = '$ofc_id'");
			extract($sel[0][0]);

			echo '<tr>';
			echo '<td width="30px">'.($no++).'.</td>';
			echo '<td>'.$ofc_customer_name.'</td>';
			echo '<td><input type="text" class="number text-right" autocomplete="off" name="amount[]" value="'.number_format($fob_amount,2).'"/>';
			echo '<input type="hidden" name="customer[]" value="'.$ofc_id.'"/>';
			echo '</td>';
			echo '</tr>';
		}
		echo '</table>';
		echo '</form>';
	}

	public function touBreakdown(){

		$year = 2020;
		$month = 8;

		?>
<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
					
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{
				$year = strtotime('-1 month', time());

				$month_uploaded = (portion(4))? portion(4) : date('m', $year);
				$year_uploaded = (portion(3))? portion(3) : date('Y', $year);
			}
			
			?>
			
		</form>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=2019; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>
		<br/>

		<?php

		$year = $year_uploaded;
		$month = $month_uploaded;
		echo '<span onclick="return print(\'printMe\');" class="pull-right btn btn-default"><i class="fa fa-print"></i> Print</span>';
		echo '<div id="printMe">';
		echo '<h4>Monthly TOU Breakdown '.$year.' '.month_name($month).'</h4>';
		echo '<p><b>Power sales by Time of Use</b></p>';
		echo '<table style="width:100%; font-size:100%;" cellpadding=0 cellspacing="0" id="table" border="1">';

		echo '<tr>';
		echo '<th width="40px">No.</th>';
		echo '<th>Customer</th>';
		echo '<th>Peak</th>';
		echo '<th>Shoulder</th>';
		echo '<th>Off Peak</th>';
		echo '<th>Total</th>';
		echo '</tr>';

		$db = new Db();
		$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
		$no=1;
		$t0=$t1=$t2=$t3;
		foreach($select[0] as $row){
			extract($row);
			$cus = $this->sumFormulae($customer_id, $year, $month);
			$t0 += $cus[0];
			$t1 += $cus[1];
			$t2 += $cus[2];
			$t3 += $cus[3];
			
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			echo '<td>'.$customer_short_name.'</td>';
			echo '<td style="text-align:right;">'.number_format($cus[0]).'</td>';
			echo '<td style="text-align:right;">'.number_format($cus[1]).'</td>';
			echo '<td style="text-align:right;">'.number_format($cus[2]).'</td>';
			echo '<td style="text-align:right;">'.number_format($cus[3]).'</td>';
			echo '</tr>';
		}

		echo '<tr>';
		echo '<th style="font-weight:bold;"></th>';
		echo '<th style="font-weight:bold;">Total</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t0).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t1).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t2).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t3).'</th>';
		echo '</tr>';
		echo '</table>';


		echo '<br/><p><b>Energy Purchases</b></p>';
		echo '<table style="width:100%; font-size:100%;" cellpadding=0 cellspacing="0" id="table" border="1">';

		echo '<tr>';
		echo '<th width="40px">No.</th>';
		echo '<th>Customer</th>';
		echo '<th>Peak</th>';
		echo '<th>Shoulder</th>';
		echo '<th>Off Peak</th>';
		echo '<th>Units</th>';
		echo '<th>Deemed Energy</th>';
		echo '</tr>';

		$db = new Db();

		$t0=$t1=$t2=$t3=0;
		$total_units = array();
		$select = $db->select("SELECT * FROM generator_category ORDER BY gc_name ASC");
		foreach($select[0] as $row){
			extract($row);
			$total_units[$gc_name]=0;
		}
		$no=1;
			foreach($select[0] as $row){
				extract($row);
				$select = $db->select("SELECT * FROM generators WHERE ge_category = '$gc_id' ORDER BY ge_name ASC");
				if($db->num_rows()){
				echo '<tr style="background-color:#999 !important;"><td></td><td colspan="6" style=""><b>'.$gc_name.'</b></td></tr>';
				
			
				foreach($select[0] as $row){
					extract($row);
					$s = new Db();
					$sql = "SELECT mix_units,mix_deemed FROM mix_price WHERE mix_generator_id = '$ge_id' AND mix_year='$year' AND mix_month='$month'";
					$ss = $s->select($sql);
					extract($ss[0][0]);
					echo '<tr>';
					echo '<td>'.($no++).'.</td>';
					echo '<td>'.$ge_name.'</td>';
					echo '<td style="text-align:right;"></td>';
					echo '<td style="text-align:right;"></td>';
					echo '<td style="text-align:right;"></td>';
					echo '<td style="text-align:right;">'.number_format($mix_units).'</td>';
					echo '<td style="text-align:right;">'.$mix_deemed.'</td>';
					echo '</tr>';
					$total_units[$gc_name] += $mix_units;
				}

			}
		}

		echo '<tr>';
		echo '<th style="font-weight:bold;"></th>';
		echo '<th style="font-weight:bold;">Total</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t0).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t1).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t2).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t2).'</th>';
		echo '<th style="text-align:right;font-weight:bold;">'.number_format(array_sum($total_units)).'</th>';
		echo '</tr>';

		echo '</table>';

		echo '<br/>';
		echo '<p><b>Monthly Report</b></p>';

		echo '<table style="width:100%; font-size:100%;" cellpadding=0 cellspacing="0" id="table" border="1">';

		echo '<tr>';
		echo '<th width="40px">No.</th>';
		echo '<th>Customer</th>';
		echo '<th>Peak</th>';
		echo '<th>Shoulder</th>';
		echo '<th>Off Peak</th>';
		echo '<th>Units</th>';
		echo '</tr>';

		$db = new Db();

		$t0=$t1=$t2=$t3=0;
		$select = $db->select("SELECT * FROM generator_category ORDER BY gc_name ASC");
		$no=1;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			echo '<td>'.$gc_name.'</td>';
			echo '<td style="text-align:right;"></td>';
			echo '<td style="text-align:right;"></td>';
			echo '<td style="text-align:right;"></td>';
			echo '<td style="text-align:right;">'.number_format($total_units[$gc_name]).'</td>';
			echo '<tr>';
		}

		echo '<table style="width:100%; font-size:100%;" cellpadding=0 cellspacing="0">'; 
		echo '</div>';

	}

	
	public function regionalSales(){

		$year = 2020;
		$month = 8;

		?>
<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
					
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{
				$year = strtotime('-1 month', time());

				$month_uploaded = (portion(4))? portion(4) : date('m', $year);
				$year_uploaded = (portion(3))? portion(3) : date('Y', $year);
			}
			
			?>
			
		</form>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=2019; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>
		<br/>

		<?php

		$z= $q; //============DIVIDER===========
		$year = $year_uploaded;
		$month = $month_uploaded;
		echo '<span onclick="return print(\'printMe\');" class="pull-right btn btn-default"><i class="fa fa-print"></i> Print</span>';
		echo '<div id="printMe">';
		echo '<h4>Monthly Regional Sales '.$year.' '.month_name($month).' (MWh)</h4>';
		//echo '<p><b>Power sales by Time of Use</b></p>';
		echo '<table style="width:100%; font-size:100%;font-size:10px;" cellpadding=0 cellspacing="0" id="table" border="1">';
		$db = new Db();
		$sql = "SELECT distinct mp_region_id  FROM r_reading, r_rate,metering_point WHERE rea_id = rate_reading_id AND rea_cus_id = rate_cus_id AND rea_cus_id = '2019' AND rea_mp_id = mp_id";
		$sel = $db->select($sql);
		$region_name = array();
		$region_id = array();
		$areas = array(1,);
		foreach($sel[0] as $row){
			extract($row);
			$rn = $this->rgf("meter_region", $mp_region_id, "region_id", "region_name");
			$ro = $this->rgf("meter_region", $mp_region_id, "region_id", "region_order");
			//if($ro <= 5)
			{
				$region_name[]=$rn;
				$region_id[] = $mp_region_id;
			}

		}

		echo '<tr>';
		echo '<th width="40px">No.</th>';
		//echo '<th>UMEME</th>';
		echo '<th>Metering Point</th>';
		for($i=0; $i<count($region_name); $i++){
			echo '<th style="width:8%;">'.$region_name[$i].'</th>';
		}
		echo '<th>Total</th>';
		echo '</tr>';

		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '2019' ORDER BY mp_location ASC");
		$no=1;
		$t0=$t1=$t2=$t3;
		$x = 0;
		$z = 1000000;
		$total_region=array();
		foreach($select[0] as $row){
			extract($row);
						
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			//echo '<td style="text-align:right;"></td>';
			echo '<td>'.$mp_location.'</td>';
			$rea_date = strtotime("$year-$month-15");
			$sql = "SELECT rate_sfe FROM r_reading, r_rate WHERE rea_id = rate_reading_id AND rea_date = '$rea_date' AND rea_mp_id = '$mp_id'";
			$r = $db->select($sql);
			extract($r[0][0]);
			$v = $this->get_advance($mp_customer_id, $mp_id, $month, $year, $rate_sfe);
			$total_region[$mp_region_id][] = $v['ATOTAL'];

			for($i=0; $i<count($region_id); $i++){
				if($region_id[$i] != $mp_region_id)
					echo '<td></td>';
				else
					echo '<td style="text-align:right">'.number_format($v['ATOTAL']/$z, 2).'</td>';
			}

			echo '<td style="text-align:right;">'.number_format($v['ATOTAL']/$z, 2).'</td>';
			echo '</tr>';
		}

		echo '<tr>';
		//echo '<th style="font-weight:bold;"></th>';
		echo '<th style="font-weight:bold;" colspan="2">Total</th>';
		$t3 = 0;
		for($i=0; $i<count($region_id); $i++){
			$t = array_sum($total_region[$region_id[$i]]);
			$t3 += $t;
			echo '<th style="text-align:right">'.number_format($t/$z, 2).'</th>';
		}

		echo '<th style="text-align:right;font-weight:bold;">'.number_format($t3/$z, 2).'</th>';
		echo '</tr>';
		echo '</table>';

		echo '<table style="width:auto; font-size:100%; font-size:10px;" cellpadding=0 cellspacing="0" id="table" border="1">';
		
		echo '<tr>';
		echo '<th colspan="2">SUMMARY</th>';
		echo '<tr/>';

		echo '<tr>';
		echo '<th>Region</th>';
		echo '<th>Amount</th>';
		echo '<tr/>';
		$vv =0;
		$i=0;

		foreach($region_id as $ri){
			$v = array_sum($total_region[$ri]);
			$vv += $v;
			echo '<tr>';
			echo '<td>'.$region_name[$i++].'</td>';
			echo '<td style="text-align:right;">'.number_format($v/$z, 2).'</td>';
			echo '</tr>';
		}
		echo '<tr>';
		echo '<th>Total:</th>';
		echo '<th style="text-align:right">'.number_format($vv/$z, 2).'</th>';
		echo '</tr>';

 
		echo '</div>';

	}

	public function transmissionLossesAction(){
				?>
<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
					
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{
				$year = strtotime('-1 month', time());

				$month_uploaded = (portion(4))? portion(4) : date('m', $year);
				$year_uploaded = (portion(3))? portion(3) : date('Y', $year);
			}
			
			?>
			
		</form>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=$fob_year; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>

		<?php

		$year = $year_uploaded;
		$month = $month_uploaded;

		$time = strtotime("$year-$month-01");
		$prev = strtotime('-1 month', $time);

		$prev_year = date('Y', $prev);
		$prev_month = date('m', $prev);
		echo '<span onclick="return print(\'printMe\');" class="pull-right btn btn-default"><i class="fa fa-print"></i> Print</span>';
		echo '<div id="side">';
		echo '<div id="printMe">';
		//echo '<style>@media print{@page {size: landscape}</style>';

		echo '<h5><center class="pull-left">STATEMENT OF TRANSMISSION VARIANCE FOR '.strtoupper(month_name($month)).' '.$year.'</center></h5>';

		echo '<table style="font-family:arial;"><tr valign="top"><td>';
		echo '<h5>Purchases</h5>';
		echo '<table id="table" cellspacing="0" cellpadding="2" border="1" style="width:auto;font-size:8px;">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="2"><center>No</center></th>';
		echo '<th rowspan="2"><center>GENERATOR<br/>/ Purchases</center></th>';
		echo '<th rowspan="2"><center>Category</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($month).' '.$year.'</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($prev_month).' '.$prev_year.'</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '</tr>';
		$db = new Db();
		$no = 1;
		$tc = array();
		$tp = array();
		$select = $db->select("SELECT ge_id, mix_id, mix_units FROM generators,mix_price WHERE mix_year = '$year' AND mix_month = '$month' AND mix_generator_id = ge_id ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);
			$tc[$ge_id] = $mix_units;
		}

		$select = $db->select("SELECT ge_id, mix_id, mix_units FROM generators,mix_price WHERE mix_year = '$prev_year' AND mix_month = '$prev_month' AND mix_generator_id = ge_id ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);
			$tp[$ge_id] = $mix_units;
		}
		$ttc = array_sum($tc);
		$ttp = array_sum($tp);
		$cus = array();
		$db = new Db();
		$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC ");
		foreach($select[0] as $row){
			extract($row);

			echo '<tr>';
			echo '<td style="'.$b.'">'.($no++).'.</td>';
			echo '<td style="'.$b.'">'.$ge_name.'</td>';
			echo '<td style="'.$b.'">'.$this->rgf('generator_category', $ge_category, 'gc_id', 'gc_name').'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tc[$ge_id]/1000).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tc[$ge_id]/$ttc*100, 1).' %</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tp[$ge_id]/1000).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tp[$ge_id]/$ttp*100, 1).' %</td>';
			echo '</tr>';
		}

		echo '<tr>';
		echo '<td></td>';
		echo '<td colspan="2" style="text-align:left; font-weight:bold;">Total Sales</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ttc/1000).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ttp/1000).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '</tr>';

		echo '</table>';


		echo '</td><td style="width:30px;">';
		echo '</td><td>';
		echo '<h5>Sales</h5>';
		echo '<table id="table" cellspacing="0" cellpadding="2" border="1" style="width:auto;font-size:8px;">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="2"><center>No</center></th>';
		echo '<th rowspan="2"><center>CUSTOMER</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($month).' '.$year.'</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($prev_month).' '.$prev_year.'</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '</tr>';

		$tot_pre = 0;
		$tot_cur = 0;

		$p = array();
		$c = array();

		$db = new Db();
		$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
		foreach($select[0] as $row){
			extract($row);		
			$g = new Services();
			$h = $g->invoiceTotalSummary($customer_id, $year, $month);
			//$tot_cur += $h[0];
			$c[$customer_id] = $h[0];

			$h = $g->invoiceTotalSummary($customer_id, $prev_year, $prev_month);
			//$tot_pre += $h[0];
			$p[$customer_id] = $h[0];
		}

		$tc = array_sum($c);
		$tp = array_sum($p);

		$db = new Db();
		$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
		$n=0;
		foreach($select[0] as $row){
			extract($row);

			echo '<tr>';
			echo '<td>'.(++$n).'.</td>';
			echo '<td style="text-align:left;">'.$customer_short_name.'</td>';
			echo '<td style="text-align:right;">'.number_format($c[$customer_id]).'</td>';
			echo '<td style="text-align:right;">'.number_format($c[$customer_id]/$tc*100, 1).' %</td>';
			echo '<td style="text-align:right;">'.number_format($p[$customer_id]).'</td>';
			echo '<td style="text-align:right;">'.number_format($p[$customer_id]/$tp*100, 1).' %</td>';
			echo '</tr>';
		}

		echo '<tr>';
		echo '<td></td>';
		echo '<td style="text-align:left; font-weight:bold;">Total Sales</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tc).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tp).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '</tr>';

		echo '</table>';

		echo '<br/>';

		echo '<h5>Total Purchases, Sales and Losses</h5>';

		echo '<table id="table" border="1" cellspacing="0" cellpadding="2" style="font-size:10px;">';

		echo '<tr>';
		echo '<th style="text-align:left;">Period</th>';
		echo '<th><center>'.month_name_short($month).' '.$year.'</center></th>';
		echo '<th><center>'.month_name_short($prev_month).' '.$prev_year.'</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Total Purchases</th>';
		echo '<th style="text-align:right;">'.number_format($ttc).'</th>';
		echo '<th style="text-align:right;">'.number_format($ttp).'</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Total Sales</th>';
		echo '<th style="text-align:right;">'.number_format($tc).'</th>';
		echo '<th style="text-align:right;">'.number_format($tp).'</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Losses</th>';
		echo '<th style="text-align:right;">'.number_format($ttc - $tc).'</th>';
		echo '<th style="text-align:right;">'.number_format($ttc - $tp).'</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Losses (%)</th>';
		echo '<th style="text-align:right;">'.number_format(($ttc - $tc)/$ttc*100, 2).' %</th>';
		echo '<th style="text-align:right;">'.number_format(($ttp - $tp)/$ttc*100, 2).' %</th>';
		echo '</tr>';

		echo '</table>';

		echo '</td></tr></table>';


		echo '</div>';
		

	}


	public function opticFibreData(){
		$db = new Db();
		$s = $db->select("SELECT TOP 1 fob_year, fob_month FROM fibre_opening_balance");
		extract($s[0][0]);


		?>
<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
					
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{
				$year = strtotime('-1 month', time());

				$month_uploaded = (portion(4))? portion(4) : date('m', $year);
				$year_uploaded = (portion(3))? portion(3) : date('Y', $year);
			}
			
			?>
			
		</form>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=$fob_year; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>
		<br/>

		<?php

		$year = $year_uploaded;
		$month = $month_uploaded;

		$fob_date = strtotime("$fob_year-$fob_month-01");

		if(strtotime("$year-$month-01") >= strtotime("$fob_year-$fob_month-01")){
		echo '<span onclick="return print(\'printMe\');" class="pull-right btn btn-default"><i class="fa fa-print"></i> Print</span>';
		echo '<div id="printMe">';

		echo '<h5><center>FIBRE BUSINESS DATA '.$year.' '.month_name($month).'</center></h5>';

		echo '<table id="table" cellspacing="0" cellpadding="2" style="width:100%;font-size:10px;" border="1">';
		echo '<tr>';
		echo '<th rowspan="2">No.</th>';
		echo '<th rowspan="2">Client</th>';
		echo '<th rowspan="2">Description of Transaction</th>';
		echo '<th rowspan="2">Balance<br/>GL</th>';
		echo '<th colspan="2">Month</th>';
		echo '<th>OUTSTANDING BALANCE</th>';
		echo '<th></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th>Issued Invoices <br/>(Usd - VAT Excl.)</th>';
		echo '<th>Realised Revenue <br/>(Usd - VAT Excl.)</th>';
		echo '<th>Amount(Usd - VAT Excl.)</th>';
		echo '<th>%</th>';
		echo '</tr>';

		$year = $year_uploaded;
		$month = $month_uploaded;

		$start_date = strtotime("$year-$month-01 12:00:00am");
		$end_date = strtotime(date("Y-m-t", $start_date).' 11:59:59pm');

		$db = new Db();
		
		$no=1;

		$total_balance = 0;
		$total_rec  = 0;
		$total_inv  = 0;
		$total_inv_balance  = 0;
		$last_rec = 0;
		$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
		foreach($select[0] as $row){
			extract($row);

			//old_system opening balance			

			$balance = $inv = 0;

			$sql = "SELECT TOP 1 fob_amount FROM fibre_opening_balance WHERE fob_customer_id = '$ofc_id' AND fob_year = '$fob_year' AND fob_month = '$fob_month'";
			$s = $db->select($sql);
			extract($s[0][0]);
			$balance += $fob_amount;

			
			$select = $db->select("SELECT fs_description FROM fibre_service WHERE fs_customer_id = '$ofc_id'");
			extract($select[0][0]);

			//=================RECEIPTS BEFORE=====================
			$ss = $db->select("SELECT rec_id FROM receipts WHERE rec_category = '4' WHERE rec_date_added < '$start_date' AND rec_date_added >= '$fob_date' AND rec_customer_id = '$ofc_id'");
			foreach($ss[0] as $s){
				extract($s);
				$qq = $db->select("SELECT sum(ra_amount) as raa FROM receipt_amount WHERE ra_rec_id = '$rec_id'");
				extract($qq);
				$last_rec += $raa;
			}
			//=================BEGIN INVOICE BEFORE=====================
			//echo  ' <b>'.FeedBack::date_fm($fob_date).'</b>';
			$sql = "SELECT not_contract_id as nid FROM notification WHERE not_start_date < '$start_date' AND not_start_date >= '$fob_date' AND not_customer_id = '$ofc_id';";	
			$a = $db->select($sql);
			$last_inv = 0;
			foreach($a[0] as $aa){
				extract($aa);
				$sql = "SELECT fs_qty as qty, fs_rate as rate, fs_cores as cores FROM fibre_service WHERE fs_customer_id = '$ofc_id' AND fs_contract = '$nid'";
				$s = $db->select($sql);
				foreach($s[0] as $r){
					extract($r);
					if(empty($cores)){ 
						$cores = 1;
					}
					$last_inv += $qty*$rate*$cores;
				}
			}
			//==================END INVOICE BEFORE =============================
			
			$balance += $last_inv - $last_rec;
			$inv = 0;
			$select = $db->select("SELECT * FROM notification WHERE not_start_date >= '$start_date' AND not_start_date <= '$end_date' AND not_customer_id = '$ofc_id'");
			//echo $db->num_rows();
			foreach($select[0] as $g){
			extract($g);
			$sel = $db->select("SELECT fs_qty, fs_rate, fs_cores FROM fibre_service WHERE fs_customer_id = '$ofc_id' AND fs_contract = '$not_contract_id'");
				foreach($sel[0] as $r){
					extract($r);
					if(empty($fs_cores)){
						$fs_cores = 1;
					}
					$inv += $fs_qty*$fs_rate*$fs_cores;
					//echo '>> '.$fs_qty*$fs_rate*$fs_cores;
				}
			}

			//=========== RECEIPTS NOW =======================================
			$ss = $db->select("SELECT rec_id FROM receipts WHERE rec_category = '4' WHERE rec_date_added >= '$start_date' AND rec_date_added <= '$end_date' AND rec_customer_id = '$ofc_id'");
			foreach($ss[0] as $s){
				extract($s);
				$qq = $db->select("SELECT sum(ra_amount) as raa FROM receipt_amount WHERE ra_rec_id = '$rec_id'");
				extract($qq);
				$last_rec += $raa;
			}
			//================================================================		
			$total_balance += $balance;
			$total_rec += $rec;
			$total_inv += $inv;
			$inv_balance = $balance+$inv-$rec;
			$total_inv_balance += $inv_balance;

			$t_ofc_customer_name[] = $ofc_customer_name;
			$t_fs_description[] = $fs_description;
			$t_balance[] = $balance;
			$t_inv[] = $inv;
			$t_rec[] = $rec;
			$t_inv_balance[] = $inv_balance;

		}

		$tp = 0;
		for($i=0; $i<count($t_ofc_customer_name); $i++){		
			$p = $t_inv_balance[$i]/array_sum($t_inv_balance)*100;
			$tp += $p;
			echo '<tr>';
			echo '<td>'.($i+1).'.</td>';
			echo '<td>'.$t_ofc_customer_name[$i].'</td>';
			echo '<td>'.$t_fs_description[$i].'</td>';
			echo '<td style="text-align:right;">'.number_format($t_balance[$i], 2).'</td>';
			echo '<td style="text-align:right;">'.number_format($t_inv[$i], 2).'</td>';
			echo '<td style="text-align:right;">'.number_format($t_rec[$i], 2).'</td>';
			echo '<td style="text-align:right;">'.number_format($t_inv_balance[$i], 2).'</td>';
			echo '<td style="text-align:right;">'.number_format($p,1).'</td>';
			echo '</tr>';
		}

		echo '<tr style="font-weight:bold;">';
		echo '<td colspan="3">Total:</td>';
		echo '<td style="text-align:right">'.number_format($total_balance,2).'</td>';
		echo '<td style="text-align:right">'.number_format($total_inv,2).'</td>';
		echo '<td style="text-align:right">'.number_format($total_rec,2).'</td>';
		echo '<td style="text-align:right">'.number_format($total_inv_balance,2).'</td>';
		echo '<td style="text-align:right">'.number_format($tp,1).'</td>';
		echo '</table>';

		echo '</div>';
		}else{
			FeedBack::error('<b>Not Captured,</b><br/> there is not opening balance entered into the system.');
		}

	}


	public function autoupload(){
		$db = new Db();
		$select = $db->select("SELECT contract_ofc_id,contract_id FROM contract");

		foreach($select[0] as $row){
			extract($row);
			$sel = $db->select("SELECT fs_id FROM fibre_service WHERE fs_customer_id = '$contract_ofc_id'");
			foreach($sel[0] as $r){
				extract($r);
				$db->update("fibre_service",["fs_contract"=>$contract_id],["fs_id"=>$fs_id]);
			}
		}
		// echo '<pre>';
		// print_r($sel);
		// echo '</pre>';

		/*
		//$customer_id=" AND rate_cus_id = '' ";

		$db = new Db();
		$select = $db->select("SELECT distinct rate_id, rea_mp_id, rate_lfe, rate_lfi, rate_sfe, rate_sfi FROM r_rate,r_reading WHERE rate_reading_id = rea_id AND rate_lfi IS NOT NULL AND rate_lfe IS NOT NULL $customer_id");

		$i=0;
		foreach($select[0] as $row){
			extract($row);

			$s=$db->select("SELECT rate_id as rid FROM r_reading,r_rate WHERE rea_id = rate_reading_id AND  rea_mp_id = '$rea_mp_id' $customer_id");
			foreach($s[0] as $r){
				extract($r);
				$i++;

				$db->update("r_rate",[
					"rate_lfe"=>$rate_lfe,
					"rate_lfi"=>$rate_lfi,
					"rate_sfe"=>$rate_sfe,
					"rate_sfi"=>$rate_sfi,
					"rate_label"=>$rate_label,
					"rate_narration"=>$rate_narration,
				],[
					"rate_id"=>$rid,
				]);
				//echo $rea_mp_id.' = ';
			}
		}

		echo 'Total Changed: '.$i;

		

		$customer_id=" AND rea_cus_id = '2019000000001' ";
		$db = new Db();
		// $select = $db->select("SELECT distinct rate_id, rea_mp_id, rate_lfe, rate_lfi, rate_sfe, rate_sfi FROM r_rate,r_reading WHERE rate_reading_id = rea_id AND rate_lfi IS NOT NULL AND rate_lfe IS NOT NULL $customer_id");

		// $i=0;
		// foreach($select[0] as $row)
		{
			//extract($row);
			echo $sql = "SELECT rea_mp_id, mp_location, rate_id as rid FROM r_reading,r_rate,metering_point WHERE rea_id = rate_reading_id AND mp_id = rea_mp_id $customer_id";
			$s=$db->select($sql);
			// echo '<pre>';
			// print_r($s);
			// echo '</pre>';
			foreach($s[0] as $r){
				extract($r);
				$i++;

				$db->update("r_rate",[
					"rate_lfe"=>"([Exports@$mp_location])",
					"rate_lfi"=>"([Imports@$mp_location])",
					"rate_sfe"=>"([I@$rea_mp_id])",
					"rate_sfi"=>"([E@$rea_mp_id])",
				],[
					"rate_id"=>$rid,
				]);
				//echo $rea_mp_id.' = ';
			}
		}

		echo 'Total Changed: '.$i;

		*/

		
	}
		
}



















