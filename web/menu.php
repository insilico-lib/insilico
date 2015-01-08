<?php
  $menu = array("Documentation", "Download");
  echo '<ul style="border-radius: 10px; margin: 0px; list-style-type: none; padding: 10px; background: #3388ac;">';
  foreach($menu as $menuitem) { 
  echo '<li style="padding: 10px; display: inline;">'.
       '<a style="color: #fff;" href='.strtolower($menuitem).'.php>'.
      $menuitem.
       '</a></li>';
  }
  echo "</ul>";
?>