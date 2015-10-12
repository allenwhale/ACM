set nocompatible              " be iMproved, required
filetype off                  " required

set rtp+=~/.vim/bundle/vundle/
call vundle#rc()
Plugin 'gmarik/vundle'
Plugin 'tpope/vim-fugitive'
Plugin 'L9'
Plugin 'Lokaltog/vim-easymotion'
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
Plugin 'valloric/YouCompleteMe'
Plugin 'scrooloose/nerdtree'
Plugin 'jistr/vim-nerdtree-tabs'
Plugin 'bling/vim-airline'
Plugin 'terryma/vim-multiple-cursors'
filetype plugin indent on     " required

"airline config
set laststatus=2
let g:airline_powerline_fonts=1
let g:airline#extensions#tabline#enabled=1
let g:airline#extensions#tabline#buffer_nr_show=1

"youcompltememe config
let g:ycm_global_ycm_extra_conf = '~/.vim/.ycm_extra_conf.py'
let g:ycm_enable_diagnostic_signs = 0
let g:ycm_key_invoke_completion = '<c-\>' 
set completeopt=menuone

"vim-easymotion config
map  / <Plug>(easymotion-sn)
omap / <Plug>(easymotion-tn)
map  n <Plug>(easymotion-next)
map  N <Plug>(easymotion-prev)

set t_Co=256
set term=screen-256color
set number
map <F5> :NERDTreeTabsToggle <CR>

set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab


set wrap
set showcmd
colorscheme torte
map  <F2> :w <CR> :call OP() <CR> 
map! <F2> <ESC> :w <CR> :call OP() <CR> <ESC>
map  <F9> :w <CR> :call CP_R() <CR> <ESC>
map! <F9> <ESC> :w <CR> :call CP_R() <CR> <ESC>
map  <HOME> ^
map! <HOME> <ESC>^i
map  <ESC>OH <HOME>
map! <ESC>OH <HOME>
map  <END> $
map  <ESC>OF <END>
map! <ESC>OF <ESC><END>a
function CP_R()

	if( &ft == 'cpp')
		let cpl = 'g++ -w -o "%:r.exe" -std=c++11 "%"' | let exc = '"./%:r.exe"'
	elseif( &ft == 'c')
		let cpl = 'gcc -w -o "%:r" -std=c99 "%"' | let exc = '"./%:r"'
	elseif( &ft == 'java')
		let cpl = 'javac "%"' | let exc = 'java "%:r"'
	elseif( &ft == 'python')
		let exc = 'python "%"'
	elseif( &ft == 'sh')
		let exc = 'sh "%"'
	endif

	let pause = 'printf "Press any key to continue..." && read -n 1 && exit'
	if !exists('exc')
		echo 'Can''t compile this filetype...'
		return
	endif
	if exists('cpl')
		let cp_r = cpl . ' && time ' . exc
	else
		let cp_r = 'time ' . exc
	endif
	"execute '!$COLORTERM -x bash -c ''' . cp_r . ';' . pause . ';exec bash'''
    execute '! clear && ' . cp_r . ' && ' . pause
endfunction

function OP()
	execute '!$COLORTERM -x gedit ' . "%" . ";"
endfunction

