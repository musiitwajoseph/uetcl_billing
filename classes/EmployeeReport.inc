<?php

Class EmployeeReport extends BeforeAndAfter{

	public $page = "EMPLOYEE REPORT";

	

	public function __construct(){

		$access = new AccessRights();

		//$access->pageAccess(user_id(), $this->page, 'V');

	}

	

	public function getLinks(){

		$page = "EMPLOYEE REPORT";

		$links = array(

			array(

				"link_name"=>"Employee Report", 

				"link_address"=>"employee-report/",

				"link_icon"=>"fa-users",

				"link_page"=>$page,

				"link_right"=>"v",

			),

		);

		

		return $links;

	}

	

	public function index(){

		$access = new AccessRights();

		$access->pageAccess(user_id(), $this->page, 'V');

		echo '<div class="col-md-12">';



echo '<form action="" method="post">';
        


    	if(isset($_POST['search'])){

    		$service = $_POST['service'];

    		$month = $_POST['month'];

    		$year = $_POST['year'];
                
            $employee = $_POST['employee'];




    		if(empty($year)||empty($month)){

    			FeedBack::error("Please select Year and Month");

    		}else{



    			if($year == "all"){

    				$start_date = strtotime('2020-10-20 12:00:00 am');

    			}elseif($year !="all" && $month=="all"){    				

    				$start_date = strtotime("$year-01-01 12:00:00 am");

    			}else{    				

    				$start_date = strtotime("$year-$month-01 12:00:00 am");

    			}



    			if($month == "all" && $year == "all"){

    				$end_date = time();

    			}elseif($month=="all"){

    				$end_date = strtotime("$year-12-01 12:00:00 am");

    				$end_date = strtotime(date('Y-m-t', $end_date).' 11:59:59 pm');

    			}else{

    				$end_date = strtotime("$year-$month-01 12:00:00 am");

    				$end_date = strtotime(date('Y-m-t', $end_date).' 11:59:59 pm');

    			}

    		}



    		// echo 'Start:'.FeedBack::date_fm($start_date);

    		// echo ' <br/>';

    		// echo 'End :'.FeedBack::date_fm($end_date);



    	}



    	if(isset($_POST['searchPeroid'])){

    		$service = $_POST['service'];

    		$from = $_POST['from'];

    		$to = $_POST['to'];

            $employee = $_POST['employee'];

    		if(empty($from) || empty($to) ){

    			FeedBack::error("Please Enter Both Dates (From and To)");

    		}elseif($from && $to){

    			$start_date = strtotime($from.' 12:00:00 am');

    			$end_date = strtotime($to.' 11:59:59 pm');

    		}



    	}



    	if(empty($start_date) && empty($end_date)){

    		$start_date = strtotime(date('Y-m-d').' 12:00:00 am');

    		$end_date = time();

    	}



    	$from = date('Y-m-d', $start_date);

    	$to = date('Y-m-d', $end_date);



        echo '<div style="float:left; margin-right:20px;">';

        echo '<label>Employee&nbsp;&nbsp;</label>';

        echo '<select name="employee">';

        echo '<option value="">All</option>';
        $db = new Db();
        $select = $db->select("SELECT * FROM sysuser ORDER BY user_surname ASC");
        foreach($select[0] as $row){
            extract($row);
            if($user_id == $employee)
                echo '<option selected="selected" value="'.$user_id.'">'.$user_surname.' '.$user_othername.'</option>';
            else
                echo '<option value="'.$user_id.'">'.$user_surname.' '.$user_othername.'</option>';
        }



        echo '</select>';

        echo '</div>';
        echo '<div style="float:left; margin-right:20px;">';

        echo '<label>Transaction Type&nbsp;&nbsp;</label>';

        echo '<select name="service">';

        echo '<option value="">All</option>';

        if($service == "Savings")

            echo '<option selected value="Savings">Savings</option>';

        else

            echo '<option value="Savings">Savings</option>';



        if($service == "Withdraw")

            echo '<option selected value="Withdraw">Withdraw</option>';

        else

            echo '<option value="Withdraw">Withdraw</option>';



        // if($service == "Charge")

        //     echo '<option value="Charge">Charge</option>';

        // else

        //     echo '<option value="Charge">Charge</option>';



        echo '</select>';

        echo '</div>';



    	echo '<div class="clearfix"></div><br/>';



    	echo '<div style="float:left; margin-right:20px;">';

    	echo '<label>Year&nbsp;&nbsp;</label>';

    	echo '<select name="year">';

    	echo '<option value="">Select</option>';

    	// if($year == "all")

    	// 	echo '<option selected value="all">All</option>';

    	// else

    	// 	echo '<option value="all">All</option>';

    	for($i=2020; $i<=date('Y'); $i++){

    		if($year == $i)

    			echo '<option selected value="'.$i.'">'.$i.'</option>';

    		else

    			echo '<option value="'.$i.'">'.$i.'</option>';

    	}

    	echo '</select>';

    	echo '</div>';

    	

    	echo '<div style="float:left; margin-right:20px;">';

    	echo '<label>Month&nbsp;&nbsp;</label>';

    	echo '<select name="month">';

    	echo '<option value="">Select</option>';

    	if($month == "all")

    		echo '<option selected value="all">All</option>';

    	else

    		echo '<option value="all">All</option>';

    	for($i=1; $i<=12; $i++){

    		if($month == $i)

    			echo '<option selected value="'.$i.'">'.$this->month_name($i).'</option>';

    		else

    			echo '<option value="'.$i.'">'.$this->month_name($i).'</option>';

    	}

    	echo '</select>';

    	echo '</div>';

    	

    	echo '<div style="float:left; margin-right:20px;">';

    	echo '<button type="submit" name="search" class="btn btn-xs btn-primary pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show &nbsp; </button>';

    	echo '</div>';



    	echo '<div style="float:left; margin-right:20px; width:50px;">&nbsp;';

    	echo '</div>';



    	echo '<div style="float:left; margin-right:20px;">';

    	echo '<label>From&nbsp;&nbsp;</label>';

    	echo '<input type="date" style="width:130px;" name="from" value="'.$from.'"><br/>';

    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px;">';

    	echo '<label>To&nbsp;&nbsp;</label>';

    	echo '<input type="date" style="width:130px;" name="to" value="'.$to.'">';

    	echo '</div>';

    	

    	echo '<div style="float:left; margin-right:20px;">';

    	echo '<button type="submit" name="searchPeroid" class="btn btn-xs btn-primary pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show &nbsp; </button>';

    	echo '</div>';



    	echo '<div class="clearfix"></div>';

    	echo '</form>';

    	echo '</div>';

    	echo '<div class="clearfix"></div>';

    	echo '<hr class="mb-5" style="margin-bottom:20px"/>';

         echo '<input type="text" name="search" placeholder="Search Client\'s Name" id="searchTerm" onkeyup="doSearch()" onClick="this.select();">';
         
        echo '<div class="clearfix"></div>';

    	if($start_date && $end_date){

    		$sd = FeedBack::date_fm($start_date);

    		$ed = FeedBack::date_fm($end_date);



    		if(empty($service))

    			$ti = strtoupper($this->full_name($employee))."'s report from $sd to $ed";

    		else

    			$ti = strtoupper($this->full_name($employee))."'s report for".$service." from $sd to $ed";



    		echo "<h5 class='pull-left'>$ti</h5>";

    	}

    	$this->start_print($ti);

    	$db = new Db();

    	if(empty($service))

    		$sql = "SELECT * FROM savings WHERE savings_type != 'Charge' AND savings_added_by = '$employee' AND savings_date_added >= '$start_date' AND savings_date_added <= '$end_date' ORDER BY savings_date_added ASC ";
    	else

    		$sql = "SELECT * FROM savings WHERE savings_type = '$service' AND savings_added_by = '$employee' AND savings_date_added >= '$start_date' AND savings_date_added <= '$end_date' ORDER BY savings_date_added ASC ";



    	$select = $db->select($sql);



				

				if(!$select){

					echo $db->error();

				}else{

					echo '<table style="font-size:12px;" cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';

					echo '<theadq>';

					echo '<tr valign="bottom">';

					echo '<th width="30px">No.</th>';

					echo '<th>Date</th>';

					echo '<th>Name</th>';

					echo '<th>Amount</th>';

					echo '<th>Transaction Type</th>';

					echo '<th>Transaction By</th>';

					echo '</tr>';

					

					echo '</thead>';

	

					echo '<tbody>';



					$db_values = array();

					$db_values[] = array('No.', 'Date', 'Name', 'Amount', 'Transaction Type');



					$i=1;

					$savings_amount_total = 0;

					foreach($select[0] as $row){

						extract($row);

						$savings_amount_total += $savings_amount;

						echo '<tr>';

						echo '<td><center>'.($i++).'.</center></td>';

						echo '<td>'.Feedback::date_fm($savings_date_added).'</td>';

						echo '<td>'.strtoupper($this->rgf("clients", $savings_customer_id, "client_id", "client_surname").' '.$this->rgf("clients", $savings_customer_id, "client_id", "client_othername")).'</td>';

						echo '<td align="right">'.number_format($savings_amount).' &nbsp; </td>';

						echo '<td>'.($savings_type).'</td>';

						echo '<td>'.$this->full_name($savings_added_by).'</td>';

						echo '</tr>';

						

						$db_values[] = array(

							($i-1),

							Feedback::date_fm($savings_date_added),

							$this->rgf("clients", $savings_customer_id, "client_id", "client_surname").' '.$this->rgf("clients", $savings_customer_id, "client_id", "client_othername"),

							

							number_format($savings_amount),

							$savings_type

							); 

					}



					echo '<tr>';

					echo '<td colspan="3" align="right">Total</td>';

					echo '<td align="right">'.number_format($savings_amount_total).' &nbsp; </td>';

					echo '<td></td>';

					echo '<td></td>';

					echo '</tr>';

					echo '</tbody>';

					

					echo '</table><br/>';

					$this->end_print();

	//////////////////////////////report step3/////////////////////////////////////

					

					$t = new TableCreator();

					$heading = $ti; //CHANGE

					$t->open($this->full_name(user_id()), $heading);

					$t->thd($db_values);

					$t->close();

					$t->results();



					$e = new Exporter();

					echo $e->getDisplay($heading, $t->results());

	////////////////////////////////////////////////////////////////////////////////////

				}



		

		echo '</div>';

	}



}