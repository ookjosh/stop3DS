Using Hugo

I'm using the docdock theme for hugo, which means everything lives inside docs/hugo/content. Every folder should have a _index.md file in it which acts as the base for all subpages (in that folder). To order them set the weight in the header of the page. 

I'm being lazy for building so here's the deal. cd to docs/hugo. Edit the site. Preview with hugo server. Once you're ready to build just run hugo. This will output the site to docs/hugo/docs/. Then copy the *contents* of that docs folder to docs/. Don't overwrite docs/hugo. The gitignore ignores docs/hugo/docs so you can leave that there. Commit and push and it will be available on github pages.

