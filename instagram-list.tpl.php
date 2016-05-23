<?php

/**
 * @file
 */
?>
<div class="instagram-block">
  <div class="instagram-wrapper">
    <ul class="instagram">
      <li class="instagram-li">
      <?php
	    foreach ($pictures as $picture) {
	  ?>	  
        <?php print $picture['image'] ?>
        <span class="instagram-details">
          <span class="instagram-count-likes">LIKES: <?php print $picture['likes']; ?></span>
          <span class="instagram-caption"><?php print $picture['caption']; ?></span>
        </span>
      <?php
  		};
      ?>
      </li>
    </ul>
  </div>
</div>