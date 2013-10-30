# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('epc', ['core','internet','applications','lte','network','netanim'])
    module.source = [
        'model/epc.cc',
        'helper/epc-helper.cc',
		'helper/ltehelper.cc',
		'model/ue-application.cc',
		'model/enb-application.cc',
		'model/mme-application.cc',
		'model/controller-application.cc',
		'model/ugw-application.cc',
		'model/lte-tag.cc',
		'helper/InitialModelHelper.cc',
		'model/session-application.cc',
		'model/signal.cc'
        ]

    module_test = bld.create_ns3_module_test_library('epc')
    module_test.source = [
        'test/epc-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'epc'
    headers.source = [
        'model/epc.h',
        'helper/epc-helper.h',
		'helper/ltehelper.h',
		'model/ue-application.h',
		'model/enb-application.h',
		'model/mme-application.h',
		'model/controller-application.h',
		'model/ugw-application.h',
		'model/lte-tag.h',
		'helper/InitialModelHelper.h',
		'model/session-application.h',
		'model/signal.h'
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

