<?php 
Class Files extends BeforeAndAfter{
	private function req_number(){
		return rand(11111, 99999);
	}


		
public function uploadedFilesAction(){

if(isset($_POST['save'])){
	$image = new Image();
	$image->upload("myfile");
	$link = $image->imageLink();
	$user_id=user_id();
	$time = time();
	if(1){

		$db = new Db();
		$insert =$db->query("INSERT INTO `op_attachment`(`at_id`, `at_url`,`at_added_by`,`at_date_added`) VALUES (NULL,'$link','$user_id','$time')");

		FeedBack::success();
		FeedBack::refresh();
	}
}
?>
	
		<div class="col-lg-12">
			<form action="" method="post" enctype="multipart/form-data">
			<div class="">
				<div class="">
					<h3>Files</h3><br/>
				</div>
				<div class="row">
					<div class="col-lg-8">			
						<div class="form-group">
							<label>Attach File</label>
							<input type="file" class="form-control" value="" id="item" style="width:100%;" name="myfile"/>
						</div>
					</div>
					<div class="col-lg-4">
						<div class="form-group">
							<label><br/><br/><br/></label>
							<input type="submit" name="save"  value="Add to List" class="btn btn-success"/> 
						</div>
					</div>					
				</div>	
			</div>
			</form>
			<div>
				<?php
				$db = new Db();
					$rows = $db->select("SELECT * FROM op_attachment ORDER BY at_date_added DESC");
					echo '<b>Total Attachment(s):</b> <span class="badge">'.$db->num_rows().'</span>';
					echo '<br/>';
					echo '<br/>';
					$i=0;
					echo '<form action="" method="post">';
					if(isset($_POST['delete'])){
						$attachment_id = $_POST['delete'];					
						$url = $_POST['url'.$attachment_id];

						$db->query("DELETE FROM op_attachment WHERE at_id = '$attachment_id'");

						unlink($url);

						FeedBack::success("Deleting Attachment. Please wait.");
						FeedBack::refresh();
					}
					echo '<div class="row">';
					foreach($rows as $row){
						$i++;
						extract($row);	
						$e = explode('.', $at_url);
						$images = array('jpeg', 'jpg', 'gif', 'png');
						echo '<div class="col-lg-3 col-md-3" style="margin-bottom:15px;">';
						echo '<div class="attachment" style="background-color:#f7f7f7; border:1px solid #ccc; border-radius:5px;padding:10px;"><div>';

						echo '<input type="hidden" name="url'.$at_id.'" value="'.return_url( ).$at_url.'"/>';

							echo '<button value="'.$at_id.'"class="attachment-delete" name="delete"><i class="fa fa-fw fa-times"></i></button>';
						if(in_array(strtolower(end($e)), $images)){
							echo  '<img style="height:100px;padding-bottom:10px;" src="'.return_url( ).$at_url.'" alt="'.$at_url.'"/>';
							$size = getimagesize(return_url().$at_url);
							echo '<br/><span class="image-details" style="">Width: <b>'.$size[0].'px</b>';
							echo '<br/>Height: <b>'.$size[1].'px</b>';
							echo '<br/>File Type: <b>'.strtoupper(end($e)).'</b>';
							//echo '<br/>Date Uploaded: <b>'.FeedBack::date_s($at_date_added).'</b>';
							//echo '<br/>Added by: <b>'.$this->full_name($at_added_by).'</b>';
							echo '</span>';
							echo '<div class="clearfix"></div>';
							echo '<label>Copy Url: <span class="copy">(Click & [Ctrl+C])</span></label>';	
						}else{
							echo 'File Type: <b>'.strtoupper(end($e)).'</b>';
							echo '<br/>';
						}		
						echo '</div>';
						echo '<input onclick="this.select();"class="form-control" value="'.return_url( ).$at_url.'" type="text"/>';	
						echo '</div>';		
						echo '</div>';

						if($i%4 == 0){
							echo '<div class="clearfix"></div>';
						}
					}
					echo '</div>';
					echo '<div class="clearfix"></div>';
					echo '</form>';
				?>
				
			</div>
		</div>
		
		
		<?php
	}
		
}
?>