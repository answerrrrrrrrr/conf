#!/bin/bash
. tests/bot-ci/scripts/common/main.sh

: ${USER:=`id -un`}
: ${HOME:=`getent passwd $USER | cut -d: -f6`}

export USER HOME

FAILED=0

if test "$TRAVIS" = true ; then
	export PATH="/opt/fish/bin:${PATH}"

	if test "$PYTHON_IMPLEMENTATION" = "CPython" ; then
		export PATH="/opt/zsh-${PYTHON_MM}${USE_UCS2_PYTHON:+-ucs2}/bin:${PATH}"
	fi

	if test -n "$USE_UCS2_PYTHON" ; then
		export LD_LIBRARY_PATH="/opt/cpython-ucs2-$UCS2_PYTHON_VARIANT/lib${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH"
		set +e
		. virtualenvwrapper.sh
		workon cpython-ucs2-$UCS2_PYTHON_VARIANT
		set -e
	fi
fi

export PYTHON="${PYTHON:=python}"
export PYTHONPATH="${PYTHONPATH}${PYTHONPATH:+:}`realpath .`"
for script in tests/run_*_tests.sh ; do
	if ! sh $script ; then
		echo "Failed $script"
		FAILED=1
	fi
done
exit $FAILED
