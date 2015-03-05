<?php include_once("pre_wrapper.php"); ?>

<p class="page-content-header">Documentation</p>
<!-- <p style="margin-left: 10px; padding-left: 10px;">
   &bull; Offline version of this documentation - <a href="get_doc_pdf"><span class="lib-name-font">insilico</span> 0.10</a>
   </p> -->
<p style="margin: 0px; text-align: right; float: right; padding-right: 10px;">
    &lArr;
    <a href="documentation.php">&uArr;</a>
    <a href="introduction.php">&rArr;</a>
</p>
<div class="page-contents-value">
    <table style="text-align: legt; margin: 20px; padding: 10px; border: 1px dotted #3388ac;">
	<tr><td><b><u>Contents</u></b></td></tr>
	<?php
	$contentlist = array("Introduction", 
			     "Getting Started",
			     "Tutorial",
			     "In Detail",
			     "Contribute",
			     "Acknowledgements",
			     "Code Reference",
			     "Library Index");
	foreach($contentlist as $contentitem) {
	    echo '<tr><td><a href="'.(str_replace(" ","_",strtolower($contentitem))).'.php">'.$contentitem.'</td></tr>'; 
	}
	?>
    </table>
</div>

<?php include_once("post_wrapper.php"); ?>
