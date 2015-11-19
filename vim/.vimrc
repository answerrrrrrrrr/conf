let mapleader = ','
colorscheme monokai

"Mac vim paste
"set clipboard+=unnamed

set nocompatible            "IMproved
syn on                      "语法支持
syn enable                  "语法高亮
set ai                      "自动缩进
set smartindent             "智能缩进
set bs=2                    "在insert模式下用退格键删除
set showmatch               "代码匹配
set laststatus=2            "总是显示,w状态行

set expandtab               "以下三个配置配合使用，设置tab和缩进空格数
set shiftwidth=4            "统一缩进为4
set tabstop=4               "Tab的宽度
set cursorline              "为光标所在行加下划线
set number                  "显示行号
set relativenumber
set autoread                "文件在Vim之外修改过，自动重新读入
set mouse=a                 "自动启用鼠标
set writebackup             "保存文件前建立备份，保存成功后删除该备份
"set nobackup                "设置无备份文件
set noswapfile              "设置无临时文件

set ignorecase              "检索时忽略大小写
set fileencodings=uft-8,gbk "使用utf-8或gbk打开文件
set hls                     "检索时高亮显示匹配项
set helplang=cn             "帮助系统设置为中文
set foldmethod=syntax       "代码折叠


"ctags 
filetype on
filetype plugin on



"" powerline config
"set rtp+=~/powerline-develop/powerline/bindings/vim
"set guifont=DejaVu\ Sans\ Mono\ for\ Powerline:h12
"set laststatus=2
"let g:Powerline_symbols = 'fancy'
"set encoding=utf-8
"set t_Co=256
"set number
"set fillchars+=stl:\ ,stlnc:\
"set term=xterm-256color
"set termencoding=utf-8
"syntax enable




"Vundle
set nocompatible              " required
filetype off                  " required
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'gmarik/Vundle.vim'
call vundle#end()            " required
filetype plugin indent on    " required



Bundle 'scrooloose/syntastic'
Bundle 'scrooloose/nerdtree'
Bundle 'scrooloose/nerdcommenter'   
Bundle 'Xuyuanp/nerdtree-git-plugin'
Bundle 'easymotion/vim-easymotion'
Bundle 'terryma/vim-multiple-cursors'
Bundle 'bling/vim-airline'
Bundle 'junegunn/vim-easy-align'
Bundle 'majutsushi/tagbar'
Bundle 'sjl/gundo.vim'
Bundle 'terryma/vim-expand-region'
Bundle 'kien/rainbow_parentheses.vim'
Bundle 'kien/ctrlp.vim'








" ============================ Plugin Details ============================



" [syntastic](plugin)(syntax)
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0

let g:syntastic_error_symbol = '✗'
let g:syntastic_warning_symbol = '⚠'
let g:syntastic_enable_balloons = 1

"let g:syntastic_cpp_include_dirs = ['/usr/include/']
"let g:syntastic_cpp_remove_include_errors = 1
"let g:syntastic_cpp_check_header = 1
"let g:syntastic_cpp_compiler = 'clang++'
"let g:syntastic_cpp_compiler_options = '-std=c++1z '
" [syntastic]$
 


" [The-NERD-tree](plugin)(file)
nnoremap <leader>n :NERDTreeToggle<CR>
let NERDTreeHighlightCursorline=1
let NERDTreeIgnore=[ '\.pyc$', '\.pyo$', '\.obj$', '\.o$', '\.so$', '\.egg$', '^\.git$', '^\.svn$', '^\.hg$', '\~$', 'node_modules']
" close vim if the only window left open is a NERDTree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") && b:NERDTreeType == "primary") | q | end
" 当不带参数打开Vim时自动加载项目树
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
" 不显示项目树上额外的信息，例如帮助、提示什么的
let NERDTreeMinimalUI=1
" [The-NERD-tree]$



" [vim-multiple-cursors](plugin)(efficiency)
"let g:multi_cursor_use_default_mapping=0
" Default mapping
"let g:multi_cursor_next_key='<C-n>'
"let g:multi_cursor_prev_key='<C-p>'
"let g:multi_cursor_skip_key='<C-x>'
"let g:multi_cursor_quit_key='<Esc>'
" [vim-multiple-cursors]$



"[EasyAlign](plugin)(efficiency)
vmap <Leader>a <Plug>(EasyAlign)
nmap <Leader>a <Plug>(EasyAlign)
"[EasyAlign]$



"[tagbar](plugin)
nmap <leader>tb :TagbarToggle<CR>
"[tagbar]$



" [RainbowParentheses](plugin)(color)
autocmd VimEnter * RainbowParenthesesActivate
autocmd Syntax * RainbowParenthesesLoadRound
autocmd Syntax * RainbowParenthesesLoadSquare
autocmd Syntax * RainbowParenthesesLoadBraces
" [RainbowParentheses]$







" ============================ key map ============================


nnoremap k gk
nnoremap gk k
nnoremap j gj
nnoremap gj j

map <C-j> <C-W>j
map <C-k> <C-W>k
map <C-h> <C-W>h
map <C-l> <C-W>l

map <C-a> ^
inoremap <C-a> <Home>
map <C-e> g_
inoremap <C-e> <End>


"buffer & tab
nmap <leader>bn :bnext<CR>
nmap <leader>bp :bprevious<CR>
nmap <leader>bd :bdelete<CR>
nmap <leader>bf :bfirst<CR>
nmap <leader>bl :blast<CR>
nmap <leader>bs :buffers<CR>

nmap <leader>tt :tabnew<CR>
nmap <leader>tn :tabnext<CR>
nmap <leader>tp :tabprevious<CR>
nmap <leader>tc :tabclose<CR>
nmap <leader>tf :tabfirst<CR>
nmap <leader>tl :tablast<CR>
nmap <leader>ts :tabs<CR>



nmap <leader>y "+y
vmap <leader>y "+y
nmap <leader>p "+p
nmap <leader>P "+P



nnoremap <F2> :set nu! nu?<CR>
nnoremap <F2> inore:set nu! nu?<CR>
nnoremap <F3> :set list! list?<CR>
nnoremap <F4> :set wrap! wrap?<CR>
set pastetoggle=<leader>p       "insert <--> paste mode, no autoindented
au InsertLeave * set nopaste
nnoremap <F6> :exec exists('syntax_on') ? 'syn off' : 'syn on'<CR>

" kj 替换 Esc
inoremap kj <Esc>

" Quickly close the current window
nnoremap <leader>q :q<CR>
" Quickly save the current file
nnoremap <leader>w :w<CR>
" Force to save
nnoremap <leader>W :w !sudo tee % > /dev/null<CR>

" select all
map <Leader>sa ggVG"

" remap U to <C-r> for easier redo
nnoremap U <C-r>

" Swap implementations of ` and ' jump to markers
" By default, ' jumps to the marked line, ` jumps to the marked line and
" column, so swap them
nnoremap ' `
nnoremap ` '

" switch # *
" nnoremap # *
" nnoremap * #

"Keep search pattern at the center of the screen."
nnoremap <silent> n nzz
nnoremap <silent> N Nzz
nnoremap <silent> * *zz
nnoremap <silent> # #zz
nnoremap <silent> g* g*zz

" remove highlight
noremap <silent><leader>/ :nohls<CR>

"Reselect visual block after indent/outdent.调整缩进后自动选中，方便再次操作
vnoremap < <gv
vnoremap > >gv

" y$ -> Y Make Y behave like other capitals
map Y y$

"Map ; to : and save a million keystrokes
" ex mode commands made easy 用于快速进入命令行
nnoremap ; :


" command mode
cnoremap <C-j> <t_kd>
cnoremap <C-k> <t_ku>
cnoremap <C-a> <Home>
cnoremap <C-e> <End>




"showcmd,  have to be set at last to avoid overriding
set sc
