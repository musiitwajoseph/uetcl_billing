<?php

Class Lock extends BeforeAndAfter{
	public function getLinks(){
		$links = array(
			array()
		);
		
		return $links;
	}
	public function allReadingsLocksAction(){
		if(isset($_POST['show'])){
			$filter_year = $_POST['filter_year'];	
			Feedback::redirect(return_url().'lock/all-readings-locks/'.$filter_year);		
		}else{
			if(portion(3)){
				$filter_year = portion(3);
			}else{
				$filter_year = date('Y');
			}
		}
		?>
		<form action="" method="post" class="search">
			<div class="pull-left" style="margin:10px;"> <b>Select Year: </b>&nbsp;  
				<select name="filter_year" style="width:80px; margin: auto 10px;">
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
			<div class="pull-left"  style="margin:10px;">
				<button name="show">Show</button>
			</div>
			<div class="clearfix"></div>
		</form>
		<?php
		echo '<table id="table" border="1">';
		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Customer</th>';
		for($i=1; $i<=12; $i++){
			echo '<th>'.month_name($i).'</th>';
		}
		echo '</tr>';

		$db = new Db();
		$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
		$no = 1;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			echo '<td>'.$customer_short_name.'</td>';

			for($i=1; $i<=12; $i++){
				echo '<td align="center">';
				if($this->readingExist($customer_id, $filter_year, $i)){
					if($this->isLocked($customer_id, $filter_year, $i)){
						echo '<span title="Double Click to Toggle" ondblclick="return lock(\''.return_url().'\','.$customer_id.', '.$filter_year.', '.$i.');"><span id="id'.$customer_id.$i.'"><i class="fa fa-2x fa-lock"></i></span></span>';
					}else{
						echo '<span title="Double Click to Toggle" ondblclick="return lock(\''.return_url().'\','.$customer_id.', '.$filter_year.', '.$i.');"><span id="id'.$customer_id.$i.'"><i style="color:red;" class="fa fa-2x fa-unlock"></i></span></span>';
					}
				}			
				echo '</td>';
			}
			echo '</tr>';
		}
		echo '</table>';
	}
}