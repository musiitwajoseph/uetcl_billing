<?php 

class Dashboard extends BeforeAndAfter{
	
	public function getLinks(){
		
		$links = array(
			array()
		);
		
		return $links;
	}
	
	public function indexAction(){
	
	?>
	
	<div class="block-header">
                <h2>DASHBOARD</h2>
            </div>

            <!-- Widgets -->
            <div class="row clearfix">
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="info-box bg-pink hover-expand-effect">
                        <div class="icon">                            
							<i class="fa fa-fx fa-circle"></i>
                        </div>
                        <div class="content">
                            <!-- <div class="text">NEW TASKS</div>
                            <div class="number count-to" data-from="0" data-to="125" data-speed="15" data-fresh-interval="20"></div> -->
                        </div>
                    </div>
                </div>
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="info-box bg-cyan hover-expand-effect">
                        <div class="icon">                            
							<i class="fa fa-fx fa-circle"></i>
                        </div>
                        <div class="content">
                            <!-- <div class="text">NEW TICKETS</div>
                            <div class="number count-to" data-from="0" data-to="257" data-speed="1000" data-fresh-interval="20"></div> -->
                        </div>
                    </div>
                </div>
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="info-box bg-light-green hover-expand-effect">
                        <div class="icon">
							<i class="fa fa-fx fa-comment"></i>
                        </div>
                        <div class="content">
                            <!-- <div class="text">NEW COMMENTS</div>
                            <div class="number count-to" data-from="0" data-to="243" data-speed="1000" data-fresh-interval="20"></div> -->
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
                            <div class="text">REGISTERED USERS</div>
                            <div class="number count-to" data-from="0" data-to="<?php echo $total; ?>" data-speed="1000" data-fresh-interval="20"></div>
                        </div>
                    </div>
                </div>
            </div>
            <!-- #END# Widgets -->

	<?php
	}

}