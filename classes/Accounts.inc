<?php

Class Accounts extends BeforeAndAfter{
	public function getLinks(){
		$links = array(
			array()
		);
		
		return $links;
	}
	public function accountStatementAction(){
		if(isset($_POST['show'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];	
			$customer = $_POST['customer'];	

			Feedback::redirect(return_url().'accounts/account-statement/'.$customer.'/'.$filter_year.'/'.$filter_month);		
		}else{
			if(portion(3)){
				$customer = portion(3);
			}else{
				$customer = "all";
			}

			if(portion(4)){
				$filter_year = portion(4);
			}else{
				$filter_year = date('Y');
			}

			if(portion(5)){
				$filter_month = portion(5);
			}else{
				$filter_month = date('m')-1;
			}
		}




		$customers = array();
        $db = new Db();
        $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $customers[$customer_id] = $customer_short_name;
        }
		?>
		<form action="" method="post" class="search">
			<div class="pull-left" style="margin:10px;"> <b>Select: </b><br/>
			</div>
			<div class="pull-left" style="margin:10px;"> <b>Year </b><br/>  
				<select name="filter_year" style="width:80px; margin: auto;">
					<option>Select</option>
					<?php 
					if(empty($filter_year)){
						$filter_year = date('Y');
					}
					for($i=2019; $i<=date('Y'); $i++){
						if($i == $filter_year)
							echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
						elseif($i == $year_uploaded)
							echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
						else
							echo '<option value="'.$i.'">'.($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="pull-left" style="margin:10px;"> <b>Month </b><br/>  
				<select name="filter_month" style="width:80px; margin: auto;">
					<option value="all">All</option>
					<?php 
					if(empty($filter_month)){
						$filter_month = date('m')-1;
					}
					for($i=1; $i<=12; $i++){
						if($i == $filter_month)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						else
							echo '<option value="'.$i.'">'.month_name($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="pull-left" style="margin:10px;"> <b>Customer </b><br/>  
				<select name="customer" style=" margin: auto;">
					<option value="all">All</option>
					<?php
					$i=0;
					foreach($customers as $id => $name){
						if($id == $customer)
							echo '<option value="'.$id.'" selected="selected">'.($name).'</option>';
						else
							echo '<option value="'.$id.'">'.($name).'</option>';
					}
					?>
				</select>
			</div>
			<div class="pull-left"  style="margin:10px;">
				&nbsp; <br/>
				<button name="show">Show</button>
			</div>
			<div class="clearfix"></div>
		</form>

		<?php
		if($filter_month == "all"){
			$month = " ";
		}else{
			$month = " ".month_name($filter_month)." ";
		}

		if($customer == "all"){
			$label = "ALL CUSTOMERS ACCOUNT STATEMENTS FOR ".$filter_year.$month;
		}else{
			$label = $this->rgf("customer", $customer, "customer_id", "customer_full_name")." ACCOUNT STATEMENT FOR ".$filter_year.$month;
		}

		$t = new Services();

		echo '<h4 style="margin-bottom:0; margin-top:20px; text-transform:uppercase;">'.$label.'Account Statement</h4>';		
		
		echo '<table id="table" border="1" style="margin-top:-30px;">';
		
		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Month</th>';
		echo '<th>REF No.</th>';
		echo '<th>Date</th>';
		echo '<th>PESS No.</th>';
		echo '<th>Narration</th>';
		echo '<th>Debit</th>';
		echo '<th>Credit</th>';
		echo '<th>Current Balance</th>';		
		echo '</tr>';

		$e = ($filter_month == "all")?12: $filter_month;
		$s = ($filter_month == "all")?1: $filter_month;

		$ss = ($customer == "all")?$customers: array($customer=>"name");
		
		$no = 1;
		$current_balance = 0;
		$ob = $this->opening_balance($ss, $filter_year, $filter_month);
		$opening_balance = $ob[0];

		$total_credit = 0;
		$total_debit = 0;
		$total_current_balance = 0;

		echo '<tr>';
		echo '<td align="right"></td>';
		echo '<td align="right"></td>';
		echo '<td align="right"></td>';
		echo '<td align="right"></td>';
		echo '<td align="right"></td>';
		echo '<td align="">OPENING BALANCE <br/>(Closing bal for '.$ob[1].' '.month_name($ob[2]).')</td>';
		echo '<td align="right">'.number_format($opening_balance).'</td>';
		echo '<td align="right"></td>';
		echo '<td align="right">'.number_format($opening_balance).'</td>';
		echo '</tr>';

		$total_current_balance += $opening_balance;
		$total_debit += $opening_balance;

		for($i=$s; $i<=$e; $i++){
			foreach($ss as $customer_id => $value){
				$tis = $t->invoiceTotalSummary($customer_id, $filter_year, $i);
				if(!empty($tis[3]))
				{
					$debit = $tis[3];

					$total_debit += $debit;					
					$credit = 0;
					//$total_credit += $credit;
					$current_balance = $total_debit - $total_credit;
					$total_current_balance += $current_balance;

					$g = $t->getAssignedInvoiceNumber($customer_id, $filter_year, $i);
					$ref_no = ($g[0])?$g[0]:"Pending Submission";
					$date = ($g[1])?Feedback::date_s($g[1]):Feedback::date_s(time());

					$narration = "Energy Sales";
					if($customer == "all"){
						$narration .= " to ".$this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
					}

					$month = date("M' y", strtotime($filter_year.'-'.$i.'-15'));
					echo '<tr>';
					echo '<td>'.($no++).'.</td>';
					echo '<td>'.$month.'</td>';
					echo '<td align="">'.$ref_no.'</td>';
					echo '<td align="">'.$date.'</td>';
					echo '<td align="right">'.'</td>';
					echo '<td>'.$narration.'</td>';
					echo '<td align="right">'.number_format($debit).'</td>';
					echo '<td align="right">'.number_format($credit).'</td>';
					echo '<td align="right">'.number_format($current_balance).'</td>';
					echo '</tr>';
				}

				$r = $this->receipt($customer_id, $filter_year, $filter_month);

				if(count($r)){
					foreach($r as $receipt){
						$month = date("M' y", strtotime($filter_year.'-'.$i.'-15'));					
						$credit = $receipt['amount'];					
						$debit = 0;					
						//$total_debit += $debit;
						$total_credit += $credit;
						$current_balance = $total_debit - $total_credit;
						$total_current_balance += $current_balance;

						echo '<tr>';
						echo '<td>'.($no++).'.</td>';
						echo '<td>'.$month.'</td>';
						echo '<td align="">'.$receipt['no'].'</td>';
						echo '<td align="">'.Feedback::date_s($receipt['date']).'</td>';
						echo '<td align="right">'.'</td>';
						echo '<td>Energy receipt</td>';
						echo '<td align="right">'.number_format($debit).'</td>';
						echo '<td align="right">'.number_format($credit).'</td>';
						echo '<td align="right">'.number_format($current_balance).'</td>';
						echo '</tr>';
					}
				}
			}
		}

		echo '<tr>';
		echo '<td align="right" style="font-weight:bold;"></td>';
		echo '<td align="right" style="font-weight:bold;"></td>';
		echo '<td align="right" style="font-weight:bold;"></td>';
		echo '<td align="right" style="font-weight:bold;"></td>';
		echo '<td align="right" style="font-weight:bold;"></td>';
		echo '<td align="" style="font-weight:bold;">Bal c/f</td>';
		echo '<td align="right" style="font-weight:bold;">'.number_format($total_debit).'</td>';
		echo '<td align="right" style="font-weight:bold;">'.number_format($total_credit).'</td>';
		echo '<td align="right" style="font-weight:bold;">'.number_format($current_balance).'</td>';
		echo '</tr>';
		
	}

	private function opening_balance($ss, $filter_year, $filter_month){
		$start_year = 2019;
		$start_month = 01;
		$customers = array();

		$no = 1;
		$current_balance = 0;
		$opening_balance = 0;

		$total_credit = 0;
		$total_debit = 0;
		$total_current_balance = 0;
		
		$total_current_balance += $opening_balance;
		$t = new Services();
		$got = array();
		for($year=$start_year; $year <= $filter_year; $year++){
			if($year == $filter_year && $filter_month == "all"){
				//$e = 12;
				//$s = 1;
				$y = $filter_year-1;
				break;
			}elseif($filter_month == 01 && $year == $filter_year){
				//$e = 12;
				//$s = 1;
				$y = $filter_year - 1;
				break;
			}else{
				$e = ($year == $filter_year)? $filter_month-1: 12;
				$s = 1;
				$y = $filter_year;
			}
			
			for($i=$s; $i<=$e; $i++){
				foreach($ss as $customer_id => $value){					
					$tis = $t->invoiceTotalSummary($customer_id, $year, $i);
					if(!empty($tis[3]))
					{
						//print_r($got);
						if($got[$customer_id] == 1){
							$c = true;
						}else{
							$got[$customer_id] = 1;
							$c = false;
						}

						if($c){
							$debit = $tis[3];
							$credit = $this->totalReceipts($customer_id, $year, $i);
							$current_balance = $debit - $credit;

							$total_debit += $debit;
							$total_credit += $credit;
							$total_current_balance += $current_balance;
						}
					
					}
				}
			}
		}

		return array($total_current_balance, $y, $e);

	}

	public function totalReceipts($customer, $filter_year, $filter_month){
		$db = new Db();

		$from = strtotime(($filter_year.'-'.$filter_month.'-01 12:00:00am'));
		$to = strtotime($filter_year.'-'.$filter_month.'-'.date('t', $from).' 11:59:59 pm');
		$sql = "SELECT sum(rec_amount) AS total FROM receipts WHERE rec_customer_id = '$customer' AND  rec_date >= '$from' AND rec_date <= '$to'";
		$select = $db->select($sql);
		extract($select[0][0]);
		$total;
		return $total;
	}

	public function receipt($customer, $filter_year, $filter_month){
		$db = new Db();

		$from = strtotime(($filter_year.'-'.$filter_month.'-01 12:00:00am'));
		$to = strtotime($filter_year.'-'.$filter_month.'-'.date('t', $from).' 11:59:59 pm');
		$sql = "SELECT * FROM receipts WHERE rec_customer_id = '$customer' AND rec_date >= '$from' AND rec_date <= '$to' ORDER BY rec_receipt_no ASC";
		$select = $db->select($sql);
		$x = array();
		if($db->num_rows()){
			foreach($select[0] as $row){
				extract($row);
				$x[] = array(
				'no'=>$rec_receipt_no, 
				'date'=>$rec_date, 
				'amount'=>$rec_amount);
			}
		}
		return $x;
	}


}

