# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('epc', ['core','internet','applications','lte','network','netanim'])
    module.source = [
        
        'model/epc.cc',            
    	'model/ue-application.cc',
    	'model/enb-application.cc',
    	'model/mme-application.cc',
    	'model/controller-application.cc',
    	'model/ugw-application.cc',
    	'model/lte-tag.cc',
        'model/signal.cc',
    	'model/session-application.cc',
        'model/handover-application.cc',
        'model/lte-epc-area.cc',
        'model/node-buffer.cc',
        'helper/epc-helper.cc',
        'helper/ltehelper.cc',
       # 'helper/InitialModelHelper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('epc')
    module_test.source = [
        'test/epc-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'epc'
    headers.source = [
        'model/epc.h',
    	'model/ue-application.h',
    	'model/enb-application.h',
    	'model/mme-application.h',
    	'model/controller-application.h',
    	'model/ugw-application.h',
    	'model/lte-tag.h',
        'model/signal.h',
    	'model/session-application.h',
        'model/handover-application.h',
        'model/lte-epc-area.h',
        'model/node-buffer.h',
        'helper/epc-helper.h',
        'helper/ltehelper.h',
        #'helper/InitialModelHelper.h'
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

