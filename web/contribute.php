<?php include_once("pre_wrapper.php"); ?>

<p class="page-content-header">Documentation</p>
<p style="margin: 0px; text-align: right; float: right; padding-right: 10px;">
    <a href="in_detail.php">&lArr;</a>
    <a href="documentation.php">&uArr;</a>
    <a href="acknowledgements.php">&rArr;</a>
</p>
<p class="page-content-sub-header">
    Contribute <br/>
    <span class="author">By Pranav Kulkarni, Collins Assisi Lab, IISER Pune</span>
</p>
<p class="page-contents-value">
    Contributions are the integral part of continuous improvement. For the same reason we have made this project open sourced to support and get inputs from the community.
    <br/><br/>
    &bull; Different ways to contribute to this library?
    <br/><br/>
    You can contribute by:
    <br/><br/>
    <i>Improving or adding documentation</i>: Documentation is the heart of any publicly available code library. It helps people understand the library and makes it easy to use. Our goal is to make library easily consumable. If you find something missing, incorrect or inappropriate in the documentation in any part of library and you know the better way to do the same, feel free to contribute your code or even thoughts.
    <br/><br/>
    <i>Adding/improving example codes</i>: If you have a working example code and proper description supporting it which you have created using the library, please contribute it so that other researchers/developers can use it.
    <br/><br/>
    <i>Improving the core library</i>: You have a better algorithm, architecture or even simple quick working code that can enhance the existing core library, please contrbute.
    <br/><br/>
    <i>Raising issues and bugs</i>: If you have found an edge case for certain input or execution environment configuration and you have done several tests for the same. Please report this on Github as an issue and it will be immediately taken care by the development team.
    <br/><br/>
    &bull; What things are needed to start contributing?
    <br/><br/>
    <i>Three things really:</i> Source code of library, your changes and git.
    <br/><br/>
    &bull; How to?
    <br/><br/>
    Make your changes. Test them thoroughly for verifying if there is any breaking change.
    <br/><br/>
    Importantly, if you are keen on getting your details correct of the copyright notice. Please add the copyright notice with your individual / laboratory / company name and email ID at start of the file that got changed or added to library. Add and commit them to your local repository (using 'git add' and 'git commit') with proper description in commit message. Create a patch (using 'git diff').
    <br/><br/>
    <i>One of the simplest ways to do it is as follows:</i>
    <br/><br/>
    - Download the compressed source from the <a href="download.php">Download</a> page and extract it into desired destination, say user's home directory.<br/>
    - Now, open your terminal and <span class="inline_code">cd</span> into the extracted directory.<br/>
    - You should track the unmodified version before making your own changes. To do that type the following command.
</p>
<p class="code">$ git init<br/>
    Initialized empty Git repository in [directory]
</p> 
<p class="page-contents-value">
    - As you have your local Git repository created, you can track the unmodified version of source by firing following commands.
</p>
<p class="code">$ git add .<br/>
    $ git commit -m "Initial Commit."<br/>
</p> 
<p class="page-contents-value">
    - You are now ready to make desired changes to the repository.<br/>
    - After you are done changing files in local repository, TEST the code for sanity. For simplicity, you can try running example codes and cross check the results.<br/>
    - Once you are satisfied with the modifications you can generate a patch by using following command. (Note: This is in the case where you have only one commit and changes are not yet commited. If you have more than one commit, use SHA for the HEAD on latest branch and SHA of very first commit - Initial Commit.)
</p>
<p class="code">$ git add .<br/>
    $ git diff > my_contribution.patch<br/>
</p>
<p class="page-contents-value">
    - Email it to one of the maintainers from the MAINTAINERS file with proper subject and body with detailed description supporting the patch.<br/>
    - Your code, if merged with the library, will be licensed under GNU GPLv3. If you have any issues or concerns put those in the mail along the patch.
</p>

<?php include_once("post_wrapper.php"); ?>
